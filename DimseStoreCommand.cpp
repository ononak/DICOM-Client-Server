#include "DimseStoreCommand.hpp"
#include <iostream>
namespace dicom {

bool DimseStoreCommand::execute(T_ASC_Association *association,
                                ServiceUserParameters scuParam,
                                DcmDataset *dataset) {

  DcmDataset *statusDetail = nullptr;
  T_DIMSE_C_StoreRQ req;
  T_DIMSE_C_StoreRSP rsp;
  DIC_UI sopClass;
  DIC_UI sopInstance;
  T_ASC_PresentationContextID presentationId;

  /* figure out which SOP class and SOP instance is encapsulated in the file */
  if (!DU_findSOPClassAndInstanceInDataSet(dataset, sopClass, sizeof(sopClass),
                                           sopInstance, sizeof(sopInstance))) {
    std::cerr << "No SOP Class & Instance UIDs in file:"<<std::endl;
    return false;
  }

  /* figure out which of the accepted presentation contexts should be used */
  DcmXfer filexfer(dataset->getOriginalXfer());

  if (filexfer.getXfer() != EXS_Unknown) {
    presentationId = ASC_findAcceptedPresentationContextID(
        association, sopClass, filexfer.getXferID());
    std::cout << "Requested sopClass : " << sopClass<<std::endl;
  } else {
    presentationId =
        ASC_findAcceptedPresentationContextID(association, sopClass);
  }

  if (presentationId == 0) {
    const char *modalityName = dcmSOPClassUIDToModality(sopClass);
    if (!modalityName) {
      modalityName = dcmFindNameOfUID(sopClass);
    }
    if (!modalityName) {
      modalityName = "Unknown SOP class";
    }
     std::cerr << "No presentation context for modality : " << modalityName
                << " : Sop class : " << sopClass<<std::endl;    
    return false;
  }

  /* prepare the transmission of data */
  bzero((char *)&req, sizeof(req));
  req.MessageID = association->nextMsgID++;
  strcpy(req.AffectedSOPClassUID, sopClass);
  strcpy(req.AffectedSOPInstanceUID, sopInstance);
  req.DataSetType = DIMSE_DATASET_PRESENT;\
  req.Priority = DIMSE_PRIORITY_LOW;

  /* transmitt data */
  auto cond = DIMSE_storeUser(
      association, presentationId, &req, nullptr, dataset, nullptr, nullptr,
      (scuParam.dimseRequestWaitTimeout == 0) ? DIMSE_BLOCKING
                                              : DIMSE_NONBLOCKING,
      scuParam.dimseRequestWaitTimeout, &rsp, &statusDetail, nullptr);

  /* dump some more general information */
  if (cond.good()) {
    std::cout << "Store request completed for SOP Class " << sopClass
             << " with peer: "
             << association->params->DULparams.calledPresentationAddress<<std::endl;
  } else {
    std::cerr << "Store request failed : " << cond.text()<<std::endl;
  }

  /* dump status detail information if there is some */
  if (statusDetail != nullptr) {
    delete statusDetail;
  }

  return cond.good() ? true : false;
}

} // namespace sp