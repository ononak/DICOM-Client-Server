#include "DcmtkScp.hpp"
#include "AcseMessageHandler.hpp"
#include "DimseRequestHandler.hpp"
#include "DimseRequestHandlerFactory.hpp"
#include <iostream>

namespace dicom {

DcmtkScp::DcmtkScp(ServiceProviderParameters params)
    : DicomScp(params), network(nullptr), continueToRun(true) {}
//------------------------------------------------------------------------------
DcmtkScp::~DcmtkScp() { stop(); }
//------------------------------------------------------------------------------
bool DcmtkScp::start() {
  initialize();
  runner = std::thread(&DcmtkScp::waitForAssociation, this);
  return true;
}

//------------------------------------------------------------------------------
bool DcmtkScp::stop() {
  std::cout << "Stopping SCP"<<std::endl;
  continueToRun = false;

  if (runner.joinable()) {
    runner.join();
  }
  cleanUp();
  return true;
}

//------------------------------------------------------------------------------
bool DcmtkScp::initialize() {
  if (!network) {
    continueToRun = true;
    // Initialize network
    auto cond =
        ASC_initializeNetwork(NET_ACCEPTOR, parameters.port,
                              parameters.networkConnectionTimeout, &network);
    if (cond.bad()) {
      network = nullptr;
      return false;
    }
    std::cout << parameters.aeTitle
             << " is initialized and waiting for association at port "
             << parameters.port<<std::endl;
  } else {
    std::cerr
        << " Scp network can not be initialized for association at port : "
        << parameters.port<<std::endl;
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
void DcmtkScp::cleanUp() {
  dropAndAbortAssociation();
  releaseNetwork();
}
//------------------------------------------------------------------------------
void DcmtkScp::releaseNetwork() {
  // clean up network resources
  if (network) {
    std::cout << "Releasing network"<<std::endl;
    ASC_dropNetwork(&network);
    network = nullptr;
  }
}
//------------------------------------------------------------------------------
void DcmtkScp::releaseAssociation() {
  // clean up all associations
  if (association) {

    if (association->DULassociation == nullptr) {
      notifyForAssociationRelease();
    } else {
      notifyForDisconnection();
    }

    std::cout << "Releasing association with : "
             << "{" << association->params->DULparams.callingPresentationAddress
             << "} :" << association->params->DULparams.callingAPTitle << " -> "
             << association->params->DULparams.calledAPTitle <<std::endl;

    ASC_acknowledgeRelease(association);
    ASC_dropAssociation(association);
    ASC_destroyAssociation(&association);
    association = nullptr;
  }
}
//------------------------------------------------------------------------------
void DcmtkScp::dropAndAbortAssociation() {
  // clean up all associations
  if (association) {
    notifyForDisconnection();

    std::cout << "Aborting association with : "
             << "{" << association->params->DULparams.callingPresentationAddress
             << "} :" << association->params->DULparams.callingAPTitle << " -> "
             << association->params->DULparams.calledAPTitle <<std::endl;

    ASC_dropAssociation(association);
    ASC_destroyAssociation(&association);
    association = nullptr;
  }
}
//------------------------------------------------------------------------------
OFCondition DcmtkScp::handleAssociation() {
  AcseMessageHandler handler;
  OFCondition cond = EC_Normal;

  if (handler.processAssociation({association, parameters})) {
    // receive a DIMSE command over the network, with a timeout of 30 second
    DcmDataset *statusDetail = nullptr;
    T_ASC_PresentationContextID presID = 0;
    T_DIMSE_Message msg;

    // start a loop to be able to receive more than one DIMSE command
    while (cond.good()) {
      cond = DIMSE_receiveCommand(
          association,
          (parameters.dimseRequestWaitTimeout == 0) ? DIMSE_BLOCKING
                                                    : DIMSE_NONBLOCKING,
          parameters.dimseRequestWaitTimeout, &presID, &msg, &statusDetail);
      if (cond.good()) {
        processCommand(&msg, statusDetail, presID);
      }
    }

    // Clean up on association termination.
    if (cond == DUL_PEERREQUESTEDRELEASE) {

      std::cout << "Peer requested association release" <<std::endl;
      releaseAssociation();

    } else if (cond == DUL_PEERABORTEDASSOCIATION) {

      std::cout << "Peer aborted association" <<std::endl;
      dropAndAbortAssociation();

    } else if (cond == DIMSE_NODATAAVAILABLE) {

      std::cout  << "No Dimse data received within timeout period. Association "
                  "will be closed "<<std::endl;
      releaseAssociation();
    } else {

      std::cerr << "Dimse error : " << cond.text() <<std::endl;
      dropAndAbortAssociation();
    }
  }
  return cond;
}

//------------------------------------------------------------------------------
void DcmtkScp::processCommand(T_DIMSE_Message *msg, DcmDataset *data,
                              T_ASC_PresentationContextID presID) {
  DimseRequestHandlerFactory::get(msg->CommandField)
      ->processCommand({association, msg, data, presID, this->getParams()});
}

//------------------------------------------------------------------------------
void DcmtkScp::waitForAssociation() {
  // Handle all incoming requests. Since
  // this activity is supposed to represent a server process, we do not want to
  // terminate this activity unless indicated by the continueToRun.
  std::cout << "Waiting for association" << std::endl;
  while (continueToRun) {
    try {
      // Wait for an association and handle the requests of
      // the calling applications correspondingly.
      // auto cond = ASC_receiveAssociation(
      //     network, &association, ASC_DEFAULTMAXPDU, nullptr, nullptr,
      //     OFFalse, DUL_NOBLOCK, parameters.associationWaitTimeout);
      auto cond = ASC_receiveAssociation(
          network, &association, ASC_DEFAULTMAXPDU, nullptr, nullptr, OFFalse,
          (parameters.associationWaitTimeout == 0) ? DUL_BLOCK : DUL_NOBLOCK,
          parameters.associationWaitTimeout);

      if (cond == EC_Normal) {
        notifyForConnection();
        std::cout << "Association request received : "
                 << association->params->DULparams.callingPresentationAddress
                 << ": " << association->params->DULparams.callingAPTitle
                 << " -> " << association->params->DULparams.calledAPTitle << std::endl;

        if (!handleAssociation().good()) {
          releaseAssociation();
        } else {
        }
      } else {
        if (cond == DUL_NOASSOCIATIONREQUEST) {

          std::cerr << cond.text() <<std::endl;
          releaseAssociation();

        } else {
          std::cout << cond.text() <<std::endl;
          dropAndAbortAssociation();
        }
      }
    } catch (const std::exception &e) {

      std::cerr << e.what() << std::endl;
    }
  }
}
//------------------------------------------------------------------------------------

void DcmtkScp::notifyForConnection() {

}
//------------------------------------------------------------------------------------

void DcmtkScp::notifyForDisconnection() {

}

void DcmtkScp::notifyForAssociationRelease() {

}

} // namespace dicom