#ifndef C_STORE_REQUEST_HANDLER_HPP
#define C_STORE_REQUEST_HANDLER_HPP
#pragma once

#include "DimseRequestHandler.hpp"
#include <mutex>
namespace dicom {

/**
 * @brief
 *
 */
class CStoreRequestHandler : public DimseRequestHandler {

private:
  static std::mutex callbackMutex;

public:
  virtual ~CStoreRequestHandler();
  bool processCommand(DimseRequestData req) override;
  static void storeScpCallback(void *callbackData,
                               T_DIMSE_StoreProgress *progress,
                               T_DIMSE_C_StoreRQ *req, char *imageFileName,
                               DcmDataset **imageDataSet,
                               T_DIMSE_C_StoreRSP *rsp,
                               DcmDataset **statusDetail);
};
} // namespace dicom

#endif