#ifndef DICOM_DATA_DEF_HPP
#define DICOM_DATA_DEF_HPP
#pragma once

#include <sstream>
#include <vector>

namespace dicom {
/**
 * @brief dicom service class provider parameters
 *
 */

struct ServiceProviderParameters {
  uint port;
  uint networkConnectionTimeout;
  uint associationWaitTimeout;
  uint dimseRequestWaitTimeout;
  std::string aeTitle;
  std::string inDirectory;
  std::vector<std::string> presentationSyntaxes;
  std::vector<std::string> transferSyntaxes;  
};
/**
 * @brief
 *
 */
struct ServiceUserParameters {
  std::string aeTitle;
  std::string serverAddr;
  uint serverPort;
  uint networkConnectionTimeout;
  uint associationWaitTimeout;
  uint dimseRequestWaitTimeout;  

  std::string presentationAddr() const {

    std::string remoteAddr(this->serverAddr);
    std::stringstream is;
    is << this->serverPort;
    std::string portString;
    is >> portString;
    remoteAddr += ":" + portString;
    return remoteAddr;
  };
};



} // namespace sp
#endif