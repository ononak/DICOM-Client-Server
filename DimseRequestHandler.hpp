#ifndef DIMSE_REQUEST_HANDLER_HPP
#define DIMSE_REQUEST_HANDLER_HPP
#pragma once

#include "DicomDataDef.hpp"
#include "dcmtk/dcmnet/assoc.h"
#include "dcmtk/dcmnet/diutil.h"

namespace dicom {

/**
 * @brief
 *
 */
struct DimseRequestData {
  T_ASC_Association *association;
  T_DIMSE_Message *msg;
  DcmDataset *data;
  T_ASC_PresentationContextID presID;
  ServiceProviderParameters serviceParameters;
};

/**
 * @brief Astract class for Dimse request handler
 *
 */
class DimseRequestHandler {

public:
  /**
   * @brief Process request
   *
   * @return true if request processed succesfully
   * @return false therwise
   */

  virtual ~DimseRequestHandler(){};
  /**
   * @brief
   *
   * @param
   */
  virtual bool processCommand(DimseRequestData) = 0;
};

} // namespace dicom

#endif