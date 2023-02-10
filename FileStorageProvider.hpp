#ifndef FILE_STORAGE_PROVIDER_HPP
#define FILE_STORAGE_PROVIDER_HPP
#pragma once

#include "dcmtk/dcmnet/diutil.h"

namespace dicom {

class FileStorageProvider {
private:
  static int counter;

public:
  virtual bool write(std::shared_ptr<DcmDataset> dataSetPtr,
                     const std::string &path, const std::string &filename = "");

private:
  bool decodeAndwriteToFile(std::shared_ptr<DcmDataset> dataSetPtr,
                            std::string &outputFile);
  std::string nameIt(std::shared_ptr<DcmDataset> dataSetPtr);
};
} // namespace dicom
#endif //