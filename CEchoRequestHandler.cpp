
#include "CEchoRequestHandler.hpp"
#include <iostream>

namespace dicom {

bool CEchoRequestHandler::processCommand(DimseRequestData req) {

  std::cout << "Received Echo Request"<<std::endl;;

  /* the echo succeeded !! */
  auto cond = DIMSE_sendEchoResponse(
      req.association, req.presID, &req.msg->msg.CEchoRQ, STATUS_Success, NULL);
  if (cond.bad()) {
    std::cerr << cond.text()<<std::endl;;
    return false;
  }
  return true;
}

} // namespace sp