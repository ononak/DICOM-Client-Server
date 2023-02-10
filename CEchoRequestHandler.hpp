#ifndef C_ECHO_REQUEST_HANDLER_HPP
#define C_ECHO_REQUEST_HANDLER_HPP
#pragma once

#include "DimseRequestHandler.hpp"

namespace dicom {
/**
 * @brief Handles Dimse C_Echo Request
 */
class CEchoRequestHandler : public DimseRequestHandler {

public:
  bool processCommand(DimseRequestData req) override;
};
} // namespace sp

#endif