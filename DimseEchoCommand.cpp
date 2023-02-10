#include "DimseEchoCommand.hpp"
#include <iostream>

namespace dicom {

bool DimseEchoCommand::execute(T_ASC_Association *association,
                               ServiceUserParameters scuParam,
                               DcmDataset *dataset) {

  DIC_US id = association->nextMsgID++; // generate next message ID
  DIC_US status;            // DIMSE status of C-ECHO-RSP will be stored here
  DcmDataset *sd = nullptr; // status detail will be stored here

  // send C-ECHO-RQ and handle response
  auto cond = DIMSE_echoUser(association, id,
                             (scuParam.dimseRequestWaitTimeout == 0)
                                 ? DIMSE_BLOCKING
                                 : DIMSE_NONBLOCKING,
                             scuParam.dimseRequestWaitTimeout, &status, &sd);
  if (cond.bad()) {

    std::cerr << cond.text()<<std::endl;
    return false;
  } else {
    if (status == STATUS_Success) {

      std::cout << "Echo completed with peer: "
               << association->params->DULparams.calledPresentationAddress
               << " status " << DU_cechoStatusString(status)<<std::endl;

    } else {
      
      std::cout << "C-ECHO failed with status code: " << status<<std::endl;;
      return false;
    }
  }

  if (sd != nullptr) {
    delete sd;
  }
  return true;
}

} // namespace sp