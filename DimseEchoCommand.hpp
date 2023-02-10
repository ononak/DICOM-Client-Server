#ifndef DIMSE_ECHO_COMMAND_HPP
#define DIMSE_ECHO_COMMAND_HPP
#pragma once

#include "DimseCommand.hpp"

namespace dicom {

class DimseEchoCommand : public DimseCommand {
private:
  /* data */
public:
  DimseEchoCommand() : DimseCommand(){};
  ~DimseEchoCommand(){};

  bool execute(T_ASC_Association *_association, ServiceUserParameters scuParam,
               DcmDataset *dataset = nullptr) override;
};

} // namespace dicom

#endif