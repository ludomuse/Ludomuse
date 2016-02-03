#ifndef _CNETWORKMANAGER_H_
#define _CNETWORKMANAGER_H_

#include "../../../../Engine/Include/CKernel.h"

#include "LmWifiDirectFacade.h"
#include "LmWifiObserver.h"

namespace LM
{

class CNetworkManager : LmWifiObserver
{
 private:
  LmWifiDirectFacade m_oWifiDirectFacade;
  CKernel* m_pKernel;

 public:

  CNetworkManager(CKernel* a_pKernel);

  // virtual void onGettingPeers(std::vector<std::string> peers);

  virtual void onReceiving(std::string s);

  // virtual void onReceiving(int i);

  // virtual void onReceiving(bool b);

  // virtual void onReceiving(long l);

  // virtual void onReceivingFile(std::string path);

  // virtual void onReceiving(double d);

  // virtual void onReceiving(float f);

  // virtual void onReceiving(char c);

  void send(std::string s);
  
  // void send(double d);

  // void send(float f);

  // void send(char c);

  // void sendByte(byte b);

  // void sendBytes(bytes bytes);


};

} // namespace LM

#endif // _CNETWORKMANAGER_H_
