#include "DicomServiceFactory.hpp"
#include <iostream>
int main(){

  dicom::ServiceProviderParameters scpParam;
  scpParam.aeTitle = "Dicom Scp Dummy";
  scpParam.associationWaitTimeout = 30;
  scpParam.dimseRequestWaitTimeout = 30;
  scpParam.networkConnectionTimeout = 30;
  scpParam.inDirectory = ".";
  scpParam.port = 23456;
  scpParam.presentationSyntaxes = {"1.2.840.10008.1.1", "1.2.840.10008.5.1.4.1.1.4"};
  scpParam.transferSyntaxes = {"1.2.840.10008.1.2.1", "1.2.840.10008.1.2.2", "1.2.840.10008.1.2","1.2.840.10008.1.2.4.50"};

    auto serviceClassProvider =
        dicom::DicomServiceFactory::getServiceProvider(scpParam);
    serviceClassProvider->start();


    int q;
    std::cin >> q;

return 0;
}