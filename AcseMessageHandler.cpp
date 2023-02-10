
#include "AcseMessageHandler.hpp"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmnet/diutil.h"
#include <vector>
#include <iostream>

namespace dicom {

bool AcseMessageHandler::processAssociation(AcseParameters acseParams) {

  T_ASC_Association *association = acseParams.association;

  /* accept association with preferred syntaxes */
  auto call_C_function_acceptContextsWithPreferredTransferSyntaxes = [&]() {
    std::vector<const char *> preContexChars;
    std::vector<const char *> transContexChars;

    for (size_t i = 0; i < acseParams.scpParameters.presentationSyntaxes.size();
         ++i) {
      preContexChars.push_back(
          acseParams.scpParameters.presentationSyntaxes[i].c_str());
    }
    for (size_t i = 0; i < acseParams.scpParameters.transferSyntaxes.size();
         ++i) {
      transContexChars.push_back(
          acseParams.scpParameters.transferSyntaxes[i].c_str());
    }

    return ASC_acceptContextsWithPreferredTransferSyntaxes(
        association->params, &preContexChars[0], preContexChars.size(),
        &transContexChars[0], transContexChars.size());
  };

  auto cond = call_C_function_acceptContextsWithPreferredTransferSyntaxes();

  if (cond.bad()) {
    std::cerr << cond.text()<<std::endl;;
    return false;
  }
  /* set our app title */
  ASC_setAPTitles(association->params, NULL, NULL,
                  acseParams.scpParameters.aeTitle.c_str());
  char buf[BUFSIZ];
  /* acknowledge or reject this association */
  cond = ASC_getApplicationContextName(association->params, buf, BUFSIZ);

  if ((cond.bad()) || strcmp(buf, UID_StandardApplicationContext) != 0) {
    std::cerr << "Bad or unsupported contex name for association"<<std::endl;
    return false;
  }

  std::cout << "Association request accepted from : "
           << "{" << association->params->DULparams.callingPresentationAddress
           << "} " << association->params->DULparams.callingAPTitle << " -> "
           << association->params->DULparams.calledAPTitle << std::endl;

  cond = ASC_acknowledgeAssociation(association);
  OFString temp_str;

  if (cond.bad()) {
    std::cerr << cond.text();
    return false;
  }
  return true;
}

} // namespace sp