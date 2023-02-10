#include "FileStorageProvider.hpp"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmnet/dimse.h"
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <stdlib.h>
#include <iostream>
#include <string>

namespace dicom {

int FileStorageProvider::counter = 0;

bool FileStorageProvider::decodeAndwriteToFile(
    std::shared_ptr<DcmDataset> dataSetPtr, std::string &outputFile) {

  // Determine the transfer syntax which shall be used to write the
  // information to the file. We always switch to the Little Endian
  // syntax, with explicit length.
  E_TransferSyntax xfer = dataSetPtr->getOriginalXfer();
  if (xfer == EXS_Unknown) {
    // No information about the original transfer syntax: This is
    // most probably a DICOM dataset that was read from memory.
    xfer = EXS_LittleEndianExplicit;
  }
  E_EncodingType encodingType = EET_ExplicitLength;

  // Create the meta-header information
  DcmFileFormat ff(dataSetPtr.get());
  ff.validateMetaInfo(xfer);

  auto cond = ff.saveFile(outputFile.c_str(), xfer, encodingType, EGL_recalcGL,
                          EPD_noChange, 0, 0, EWM_fileformat);

  return cond.good() ? true : false;
}

//----------------------------------------------------------------------------------
bool FileStorageProvider::write(std::shared_ptr<DcmDataset> dataSetPtr,
                                const std::string &path,
                                const std::string &filename) {
  OFString studyInstance;
  dataSetPtr->findAndGetOFString(DCM_StudyInstanceUID, studyInstance);

  auto fullpath = path + std::string{"/"} + std::string{"/"} + studyInstance.c_str();
  // create a directory for the study if it does not exist
  if (!boost::filesystem::exists(fullpath)) {
    boost::system::error_code err;
    if (!boost::filesystem::create_directories(fullpath, err)) {
      std::cerr << err.message()<<std::endl;
      return false;
    }
  }

  std::string outputFile;
  if (!filename.empty()) {
    outputFile = fullpath + std::string{"/"} + std::string{filename};
  } else {

    // generate unique filename
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid1 = generator();
    std::string uidstr = boost::uuids::to_string(uuid1);

    outputFile += fullpath + std::string{"/"} + nameIt(dataSetPtr);
  }

  return decodeAndwriteToFile(dataSetPtr, outputFile);
}

std::string
FileStorageProvider::nameIt(std::shared_ptr<DcmDataset> dataSetPtr) {

  std::string filename;
  OFString modality("X"), sopInstance("X");

  // generate unique filename
  boost::uuids::random_generator generator;
  boost::uuids::uuid uuid1 = generator();
  std::string uidstr = boost::uuids::to_string(uuid1);

  dataSetPtr->findAndGetOFString(DCM_Modality, modality);
  filename += modality.c_str();
  filename += "_";

  if (dataSetPtr->findAndGetOFString(DCM_SOPInstanceUID, sopInstance)
          .good()) {
    std::string tmp(sopInstance.c_str());
    std::replace(tmp.begin(), tmp.end(), ' ', '_');

    filename += tmp;
  } else {
    filename += uidstr.c_str();
  }
  return filename + std::string(".dcm");
}

} // namespace dicom