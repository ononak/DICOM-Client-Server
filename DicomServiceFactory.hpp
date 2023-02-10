#ifndef DICOM_SERVICE_FACTORY_HPP
#define DICOM_SERVICE_FACTORY_HPP


#include "DicomScp.hpp"
#include "DicomScu.hpp"
#include <memory>

namespace dicom {

class DicomServiceFactory {
public:
  static std::shared_ptr<DicomScu> getServiceUser(ServiceUserParameters params);
  static std::shared_ptr<DicomScp>
  getServiceProvider(ServiceProviderParameters params);
};

} // namespace dicom

#endif