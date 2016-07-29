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

  CNetworkManager(CKernel* a_pKernel, bool a_bIsServer);

  virtual void onGettingPeers(std::vector<std::string> a_vPeers);

  virtual void onReceiving(std::string s);

  void onReceivingMsg(bytes byteArray);

  // virtual void onReceiving(int i);

  // virtual void onReceiving(bool b);

  // virtual void onReceiving(long l);

  // virtual void onReceivingFile(std::string path);

  // virtual void onReceiving(double d);

  // virtual void onReceiving(float f);

  // virtual void onReceiving(char c);

  void Send(const std::string& s);

  void DiscoverPeers();

  void ConnectTo(const std::string& a_sDeviceName);
  
  // void send(double d);

  // void send(float f);

  // void send(char c);

  // void sendByte(byte b);

  void Send(bytes bytes);


};

} // namespace LM

#endif // _CNETWORKMANAGER_H_
