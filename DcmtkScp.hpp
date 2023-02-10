
#ifndef DCMTK_SCP_HPP
#define DCMTK_SCP_HPP
#pragma once

#include "DicomScp.hpp"
#include "dcmtk/dcmnet/assoc.h"
#include "dcmtk/dcmnet/diutil.h"
#include <thread>

namespace dicom {

/**
 * @brief DICOM Service Class Provider implementation based-on Dcmtk libray
 *
 */
class DcmtkScp : public DicomScp {
private:
  T_ASC_Network *network;
  T_ASC_Association *association;
  bool continueToRun;
  std::thread runner;

public:
  DcmtkScp(ServiceProviderParameters params);
  ~DcmtkScp();
  bool start() override;
  bool stop() override;

private:
  /**
   * @brief Listen incomming association requests
   */
  void waitForAssociation();
  /**
   * @brief Initialize service class provider resources
   * @return true ifinitialization completed successfully
   * @return false in case of error
   */
  bool initialize();
  /**
   * @brief Release all resources
   */
  void cleanUp();
  /**
   * @brief Release network resources
   */
  void releaseNetwork();
  /**
   * @brief Release association related resources
   */
  void releaseAssociation();
  /**
   * @brief Abort association and free related resources
   */
  void dropAndAbortAssociation();
  /**
   * @brief Process incomming association request
   * @param association
   * @return true if association accepted
   * @return false otherwise
   */
  OFCondition handleAssociation();
  /**
   * @brief
   * @param msg
   * @param data
   * @param presID
   */
  void processCommand(T_DIMSE_Message *msg, DcmDataset *data,
                      T_ASC_PresentationContextID presID);
    void notifyForConnection();
    void notifyForDisconnection();
    void notifyForAssociationRelease();
};

} // namespace sp
#endif