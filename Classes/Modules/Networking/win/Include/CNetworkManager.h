#ifndef _CNETWORKMANAGER_H_
#define _CNETWORKMANAGER_H_

#include "../../../../Engine/Include/CKernel.h"

namespace LM
{

class CNetworkManager
{

 private:
  CKernel* m_pKernel;
  
 public:

  CNetworkManager(CKernel* a_pKernel);
  
  void Send(const std::string& s);
  void DiscoverPeers();
  void ConnectTo(const std::string& a_sDeviceName);
  
  
};

} // namespace LM

#endif /* _CNETWORKMANAGER_H_ */
