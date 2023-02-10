#ifndef DIMSE_COMMAND_HPP
#define DIMSE_COMMAND_HPP
#pragma once

#include "DicomDataDef.hpp"
#include "dcmtk/dcmnet/assoc.h"
#include "dcmtk/dcmnet/diutil.h"

namespace dicom {

class DimseCommand {
public:
  DimseCommand(){};
  virtual ~DimseCommand(){};

  virtual bool execute(T_ASC_Association *association,
                       ServiceUserParameters scuParam,
                       DcmDataset *dataset = nullptr) = 0;
};

} // namespace dicom

#endif