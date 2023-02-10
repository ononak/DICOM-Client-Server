
#include "DicomServiceFactory.hpp"
#include "DcmtkScp.hpp"
#include "DcmtkScu.hpp"


namespace dicom {

std::shared_ptr<DicomScu> DicomServiceFactory::getServiceUser(ServiceUserParameters params){

  return std::shared_ptr<DicomScu>(new DcmtkScu(params));
}
//------------------------------------------------------------------------------------

std::shared_ptr<DicomScp> DicomServiceFactory::getServiceProvider(ServiceProviderParameters params){

  return std::shared_ptr<DicomScp>(new DcmtkScp(params));
}

}