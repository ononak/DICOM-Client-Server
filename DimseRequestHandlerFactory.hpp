
#ifndef DIMSE_REQUEST_HANDLER_FACTORY_HPP
#define DIMSE_REQUEST_HANDLER_FACTORY_HPP
#pragma once

#include "DimseRequestHandler.hpp"
#include <memory>

namespace dicom {

using DimseRequestHandlerPtr = std::unique_ptr<DimseRequestHandler>;
/**
 * @brief
 *
 */
class DimseRequestHandlerFactory {

public:
  /**
   * @brief
   *
   * @param
   */
  static DimseRequestHandlerPtr get(T_DIMSE_Command);
};

} // namespace dicom

#endif