#ifndef ACSE_MESSAGE_HANDLER_HPP
#define ACSE_MESSAGE_HANDLER_HPP
#pragma once

#include "DicomDataDef.hpp"
#include "dcmtk/dcmnet/assoc.h"


namespace dicom {
/**
 * @brief
 *
 */
struct AcseParameters {

  T_ASC_Association *association;
  ServiceProviderParameters scpParameters;
};

/**
 * @brief class to handle Acse messages
 *
 */
class AcseMessageHandler {

public:
  /**
   * @brief Process message
   *
   * @return true if request processed succesfully
   * @return false o/w
   */
  virtual bool processAssociation(AcseParameters acseParams);
};

} // namespace sp
#endif