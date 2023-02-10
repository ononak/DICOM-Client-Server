#ifndef DICOM_SCP_HPP
#define DICOM_SCP_HPP
#pragma once

#include "DicomDataDef.hpp"

namespace dicom {

/**
 * @brief
 *
 */
class DicomScp {
protected:
  ServiceProviderParameters parameters;

public:
  /**
   *
   */
  DicomScp(ServiceProviderParameters param) : parameters(param){};
  /**
   *
   */
  virtual ~DicomScp() {}
  /**
   */
  DicomScp(const DicomScp &scp) = delete;
  /**
   */
  DicomScp &operator=(const DicomScp &scp) = delete;
  /**
   * @brief Start the service provider service to handle the association
   * requests
   *
   */
  virtual bool start() = 0;
  /**
   * @brief Stop the service provider and release all resources
   *
   */
  virtual bool stop() = 0;
  /**
   * @brief Returns Scp parameters
   *
   */
  const ServiceProviderParameters getParams() const { return parameters; }
};
} // namespace sp

#endif