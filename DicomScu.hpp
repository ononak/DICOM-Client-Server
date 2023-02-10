#ifndef DICOM_SCU_HPP
#define DICOM_SCU_HPP
#pragma once

#include "DicomDataDef.hpp"
#include <memory>

namespace dicom {

/**
 * @brief
 *
 */
class DicomScu {

protected:
  ServiceUserParameters parameters;
public:
  /**
   */
  DicomScu(ServiceUserParameters param) : parameters(param){};
  DicomScu(){};
  virtual ~DicomScu(){};
  DicomScu(const DicomScu &scp) = delete;
  DicomScu &operator=(const DicomScu &scp) = delete;
  /**
   * @brief Start the service user to make the association with a service
   * provider
   */
  virtual bool start() = 0;
  /**
   * @brief Stop service user and disconnect from the server.
   */
  virtual bool stop() = 0;
  /**
   * @brief echo request
   */
  virtual bool echo() = 0;
  /**
   * @brief This function will read all the information from the given file,
   * figure out a corresponding presentation context which will be used
   * to transmit the information over the network to the SCP, and it
   * will finally initiate the transmission of all data to the SCP.
   *
   * @param
   *   fname - [in] Name of the file which shall be processed.
   */
  virtual bool store(std::string filename) = 0;
  const ServiceUserParameters getParams() const { return parameters; }
  void setParams(ServiceUserParameters param) { parameters = param; }
};

} // namespace dicom

#endif