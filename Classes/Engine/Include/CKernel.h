#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CNode.h"
#include "SUser.h"


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
  SUser* m_pLocalPlayer;
  SUser* m_pDistantPlayer;

  bool m_bCoopWaiting;

 public:
  CKernel();
  virtual ~CKernel();
  /// \returns the behavior tree
  CNode* GetBehaviorTree();

  CJsonParser* GetJsonParser();

  /// \biref Add the m_sSceneID to a_iPlayerID
  void AddSceneID(int a_iPlayerID, const std::string& m_sSceneID);

  /// \brief checks if the current player has this scene in his list
  bool PlayerHasScene(const std::string& a_sSceneID);

  int GetCurrentPlayer();

  /// \brief Initialize the kernel and the behavior tree
  void Init();

  /// \brief find the CEntityNode under the a_pTouch touch event and listening to a_rEvent
  /// \returns the found entity, nullptr otherwise
  CEntityNode* FindEntity(cocos2d::Touch* a_pTouch, const std::string& a_rEvent);
  void LogMessage(const std::string& a_sMessage);


  ////////////////// input callbacks
  bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);


  ////////////////// json callbacks
  /// \brief go to the next scene in the tree
  void NavNext(cocos2d::Ref* pSender, CEntityNode* a_pTarget);
  /// \brief go to the previous scene in the tree
  void NavPrevious(cocos2d::Ref* pSender, CEntityNode* a_pTarget);

  void GotoScreenID(CEvent a_rEvent, CEntityNode* a_pTarget);
  void ValidateScene(CEvent a_rEvent, CEntityNode* a_pTarget);
  void Validate(CEvent a_rEvent, CEntityNode* a_pTarget);
  void SetNodeVisible(CEvent a_rEvent, CEntityNode* a_pTarget);
  void FadeEntity(CEvent a_rEvent, CEntityNode* a_pTarget);
  void SetPlayerID(CEvent a_rEvent, CEntityNode* a_pTarget);
  void SendNetworkMessage(CEvent a_rEvent, CEntityNode* a_pTarget);
  void Connect(CEvent a_rEvent, CEntityNode* a_pTarget);
  void DisableEvent(CEvent a_rEvent, CEntityNode* a_pTarget);
  void EnableEvent(CEvent a_rEvent, CEntityNode* a_pTarget);
  void AnchorEntityCallback(CEvent a_rEvent, CEntityNode* a_pTarget);

  //////////////// network callbacks
  void OnReceivingMessage(const std::string& a_rMessage);
  void OnReceiving(bytes byteArray);
  void GetPeers();
  void OnGettingPeers(const std::vector<std::string>& a_vPeers);


private:
	void AnchorEntity(CEntityNode* a_pAnchorEntity, CEntityNode* a_pAnchoredEntity);

};


} // namespace LM

#endif /* _CKERNEL_H_ */
