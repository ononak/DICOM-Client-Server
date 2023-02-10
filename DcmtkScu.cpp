#include "DcmtkScu.hpp"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include <vector>
#include <iostream>

namespace dicom {

DcmtkScu::DcmtkScu(ServiceUserParameters params) : DicomScu(params) {
  initialize();
}
DcmtkScu::DcmtkScu() : DicomScu() { initialize(); }

void DcmtkScu::initialize() {
  network = nullptr;
  associationParameters = nullptr;
  association = nullptr;
  echoCommand = DimseCommandPtr(new DimseEchoCommand());
  storeCommand = DimseCommandPtr(new DimseStoreCommand());
}
//------------------------------------------------------------------------------
DcmtkScu::~DcmtkScu() { stop(); }

//------------------------------------------------------------------------------
bool DcmtkScu::start() {

  auto constatus = ASC_initializeNetwork(
      NET_REQUESTOR, 0, parameters.networkConnectionTimeout, &network);

  if (!constatus.good()) {

    std::cout << "Scu Failed to connect network"<< std::endl;
    return false;
  }
  ASC_createAssociationParameters(&associationParameters, ASC_DEFAULTMAXPDU);

  // set calling and called AE title
  ASC_setAPTitles(associationParameters, parameters.aeTitle.c_str(), "ANY-SCP",
                  NULL);

  ASC_setPresentationAddresses(associationParameters,
                               this->parameters.serverAddr.c_str(),
                               this->parameters.presentationAddr().c_str());

  // list of transfer syntaxes
  std::vector<const char *> transferSyntaxes = {
      UID_LittleEndianExplicitTransferSyntax,
      UID_BigEndianExplicitTransferSyntax,
      UID_LittleEndianImplicitTransferSyntax, UID_JPEGProcess1TransferSyntax};

  // add presentation context to association request
  // IMPORTANT : only odd numbers are alowed
  ASC_addPresentationContext(associationParameters, 1, UID_VerificationSOPClass,
                             &transferSyntaxes[0], 4);
  ASC_addPresentationContext(associationParameters, 3, UID_MRImageStorage,
                             &transferSyntaxes[0], 4);
  // ASC_addPresentationContext(associationParameters, 5,
  // UID_UltrasoundMultiframeImageStorage, &transferSyntaxes[0], 4);

  std::cout << this->getParams().aeTitle << " Initiating association with "
            << associationParameters->DULparams.calledPresentationAddress << std::endl;

  OFCondition cond = ASC_requestAssociation(
      network, associationParameters, &association, nullptr, nullptr,
      (parameters.associationWaitTimeout == 0) ? DUL_BLOCK : DUL_NOBLOCK,
      parameters.associationWaitTimeout);

  // request DICOM association
  if (cond.good()) {
    std::cout << "Association established with "
             << association->params->DULparams.calledPresentationAddress<< std::endl;

    if (ASC_countAcceptedPresentationContexts(associationParameters) >= 1) {
      return echo();
    } else {

      std::cerr << "But no valid presentation contexts"<< std::endl;
      return false;
    }
  } else {

    std::string errorText(cond.text());
    std::replace(errorText.begin(), errorText.end(), '\n', ' ');
    std::cerr << errorText << std::endl;
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DcmtkScu::stop() {

  if (association) {
    ASC_releaseAssociation(association);  // release association
    ASC_destroyAssociation(&association); // delete assoc structure
    association = nullptr;
  }
  if (network) {
    ASC_dropNetwork(&network); // delete net structure
    network = nullptr;
  }

  return true;
}

//--------------------------------------------------------------------
bool DcmtkScu::echo() { return echoCommand->execute(association, parameters); }
//-----------------------------------------------------------------------
bool DcmtkScu::store(std::string filename) {

  /* read information from file. After the call to DcmFileFormat::loadFile(...)
   * the information */
  /* which is encapsulated in the file will be available through the
   * DcmFileFormat object. */
  /* In detail, it will be available through calls to
   * DcmFileFormat::getMetaInfo() (for */
  /* meta header information) and DcmFileFormat::getDataset() (for data set
   * information). */
  DcmFileFormat dcmff;
  OFCondition cond = dcmff.loadFile(filename.c_str());

  if (cond.bad()) {
    std::cerr << "Bad DICOM file: " << cond.text()<<std::endl;
    return false;
  }

  auto completed = storeCommand->execute(association, parameters, dcmff.getDataset());

  if (completed) {
  notifyForFileTransfer();
  }

return completed;
}

void DcmtkScu::notifyForFileTransfer() {

}

} // namespace sp