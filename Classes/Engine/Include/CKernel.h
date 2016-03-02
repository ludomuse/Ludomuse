#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CNode.h"


namespace LM 
{

class CInputManager;
class CJsonParser;
class CNetworkManager;
struct CEvent;
class CEntityNode;

/// \class CKernel
/// \ingroup Engine
/// \brief the kernel of the game engine. Containing the behavior tree
class CKernel
{
 private:
  /// \brief the Behavior Tree of the game
  /// \details a pointer to the root node of the tree, usually a SequenceNode
  CNode* m_pBehaviorTree;
  /// \details will forward the inputs to the behavior tree
  CInputManager* m_pInputManager;
  /// \details will manage networking events such as direct wifi
  CNetworkManager* m_pNetworkManager;
  
  /// \brief The parser that will build the behavior tree from the json file
  CJsonParser* m_pJsonParser;

  std::map<int, std::vector<std::string> > m_mScenesID;
  int m_iPlayerID;

 public:
  CKernel();
  virtual ~CKernel();
  /// \returns the behavior tree
  CNode* GetBehaviorTree();

  CJsonParser* GetJsonParser();

  void AddSceneID(int a_iPlayerID, const std::string& m_sSceneID);

  bool PlayerHasScene(const std::string& a_sSceneID);

  /// \brief Initialize the kernel and the behavior tree
  void Init();

  ////////////////// callbacks
  void NavNext(cocos2d::Ref* pSender);
  void NavPrevious(cocos2d::Ref* pSender);

  bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

  void GotoScreenID(CEvent a_rEvent);

  void ValidateScene(CEvent a_rEvent);

  void SetNodeVisible(CEvent a_rEvent);

  void SetPlayerID(CEvent a_rEvent);

  CEntityNode* FindEntity(cocos2d::Touch* a_pTouch, const std::string& a_rEvent);

  void SendNetworkMessage(CEvent a_rEvent);
  void OnReceivingMessage(const std::string& a_rMessage);
  void GetPeers();
  void OnGettingPeers(const std::vector<std::string>& a_vPeers);
  void Connect(CEvent a_rEvent);

  void LogMessage(const std::string& a_sMessage);
};


} // namespace LM

#endif /* _CKERNEL_H_ */
