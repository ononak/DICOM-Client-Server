#ifndef DIMSE_STORE_COMMAND_HPP
#define DIMSE_STORE_COMMAND_HPP
#pragma once

#include "DimseCommand.hpp"

namespace dicom {

class DimseStoreCommand : public DimseCommand {
public:
  DimseStoreCommand() : DimseCommand(){};
  ~DimseStoreCommand(){};

  bool execute(T_ASC_Association *_association, ServiceUserParameters scuParam,
               DcmDataset *dataset = nullptr) override;
};

} // namespace sp

#endif