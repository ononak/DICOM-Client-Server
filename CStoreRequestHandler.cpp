
#include "CStoreRequestHandler.hpp"
#include "FileStorageProvider.hpp"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmnet/dimse.h"
#include <mutex>
#include <iostream>
namespace dicom {

std::mutex CStoreRequestHandler::callbackMutex;

CStoreRequestHandler::~CStoreRequestHandler() {
  std::cout << "Deleting CStoreRequestHandler"<<std::endl;
}

bool CStoreRequestHandler::processCommand(DimseRequestData req) {

  std::cout << "Received Store Request"<<std::endl;

  T_DIMSE_C_StoreRQ *cStoreReq;
  // assign the actual information of the C-STORE-RQ command to a local variable
  cStoreReq = &req.msg->msg.CStoreRQ;
  DcmFileFormat dcmff;

  // store SourceApplicationEntityTitle in metaheader
  if (req.association && req.association->params) {
    const char *aet = req.association->params->DULparams.callingAPTitle;
    if (aet)
      dcmff.getMetaInfo()->putAndInsertString(DCM_SourceApplicationEntityTitle,
                                              aet);
  }

  auto cond = DIMSE_storeProvider(
      req.association, req.presID, cStoreReq, nullptr, OFFalse, &req.data,
      &CStoreRequestHandler::storeScpCallback, &req.serviceParameters,
      (req.serviceParameters.dimseRequestWaitTimeout == 0) ? DIMSE_BLOCKING
                                                           : DIMSE_NONBLOCKING,
      req.serviceParameters.dimseRequestWaitTimeout);

  return cond.good() ? true : false;
}

//-------------------------------------------------------------------------------------------------

// TODO Add mutex
// TODO Move file saving to another workerer queue
void CStoreRequestHandler::storeScpCallback(
    void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req,
    char *imageFileName, DcmDataset **imageDataSet, T_DIMSE_C_StoreRSP *rsp,
    DcmDataset **statusDetail) {

   std::unique_lock<std::mutex> lock(callbackMutex);

  if (progress->state == DIMSE_StoreBegin) {

    std::cout << "Incomming file transfer started"<<std::endl;

  } else if (progress->state == DIMSE_StoreProgressing) {

    std::cout << "Total bytes received = " << progress->progressBytes<<std::endl;

  } else if (progress->state == DIMSE_StoreEnd) {

    std::shared_ptr<DcmDataset> receivedDataPtr(*imageDataSet);
    ServiceProviderParameters *scpParam =
        (ServiceProviderParameters *)callbackData;
    FileStorageProvider storage;
    storage.write(receivedDataPtr, scpParam->inDirectory);

    rsp->DimseStatus = STATUS_Success;

    OFString modality, study, serie, acquisition, instance;
    receivedDataPtr->findAndGetOFString(DCM_Modality, modality);
    receivedDataPtr->findAndGetOFString(DCM_StudyInstanceUID, study);
    receivedDataPtr->findAndGetOFString(DCM_SeriesNumber, serie);
    receivedDataPtr->findAndGetOFString(DCM_AcquisitionNumber, acquisition);
    receivedDataPtr->findAndGetOFString(DCM_InstanceNumber, instance);

    std::cout << " Received DICOM file : Modality : " << modality.c_str()
             << " Study : " << study.c_str() << " Serie : " << serie.c_str()
             << " Acquisition : " << acquisition.c_str()
             << " Instance : " << instance.c_str() <<std::endl;;

    if (*statusDetail != nullptr) {
      delete statusDetail;
    }
  }
}
} // namespace dicom