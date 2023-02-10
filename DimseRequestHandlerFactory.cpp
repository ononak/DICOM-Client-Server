#include "DimseRequestHandlerFactory.hpp"
#include "CEchoRequestHandler.hpp"
#include "CStoreRequestHandler.hpp"
#include <memory>

namespace dicom {

DimseRequestHandlerPtr DimseRequestHandlerFactory::get(T_DIMSE_Command type) {

  DimseRequestHandlerPtr handler(nullptr);

  switch (type) {
  case DIMSE_C_ECHO_RQ:
    handler = DimseRequestHandlerPtr(new CEchoRequestHandler());
    break;
  case DIMSE_C_STORE_RQ:
    handler = DimseRequestHandlerPtr(new CStoreRequestHandler());
    break;

  default:
    break;
  }

  return handler;
}

} // namespace sp