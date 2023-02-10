#ifndef DCMTK_SCU_HPP
#define DCMTK_SCU_HPP
#pragma once

#include "DicomScu.hpp"
#include "DimseEchoCommand.hpp"
#include "DimseStoreCommand.hpp"
#include "dcmtk/dcmnet/assoc.h"
#include "dcmtk/dcmnet/diutil.h"

namespace dicom {

/**
 * @brief DICOM Service Class User implementation based-on Dcmtk libray
 *
 */

using DimseCommandPtr = std::unique_ptr<DimseCommand>;

class DcmtkScu : public DicomScu {
private:
  T_ASC_Network *network; // network struct, contains DICOM upper layer FSM etc.
  T_ASC_Parameters *associationParameters; // parameters of association request
  T_ASC_Association *association;
  DimseCommandPtr echoCommand;
  DimseCommandPtr storeCommand;

public:
  DcmtkScu(ServiceUserParameters params);
  DcmtkScu();
  ~DcmtkScu();
  bool start() override;
  bool stop() override;
  bool echo() override;
  bool store(std::string filename) override;

private:
  void initialize();
  void notifyForFileTransfer();
};

} // namespace spm

#endif