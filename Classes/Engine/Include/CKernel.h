﻿#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CSceneNode.h"
#include "SUser.h"
#include "../../Modules/Util/Include/CStats.h"

namespace LM 
{

class CInputManager;
class CJsonParser;
class CNetworkManager;
class CSoundManager;
struct SEvent;
class CEntityNode;
class CTouchBeganVisitor;

/// \class CKernel
/// \ingroup Engine
/// \brief the kernel of the game engine. Containing the behavior tree
class CKernel
{
 private:
  
  /// \brief The parser that will build the behavior tree from the json file
  CJsonParser* m_pJsonParser;

  std::map<int, std::vector<std::string> > m_mScenesID;

  std::map<int, CTouchBeganVisitor> m_mTouchBeganVisitors;

  bool m_bIsServer;

  CSerializableStats* m_pRemoteStats;

  std::mutex m_oCountdownMutex;


public:
	/// \brief a reference to the dashboard with the timeline of the game
	CSceneNode* m_pDashboard;

	/// \brief the Behavior Tree of the game
	/// \details a pointer to the root node of the tree, usually a SequenceNode
	CNode* m_pBehaviorTree;

	CSceneNode* m_pCurrentScene;

	CSceneNode* m_pWaitingScene;

	/// \details will forward the inputs to the behavior tree
	CInputManager* m_pInputManager;
	/// \details will manage networking events such as direct wifi
	CNetworkManager* m_pNetworkManager;

	CSoundManager* m_pSoundManager;

	bool m_bDebugMode;

	SUser* m_pLocalPlayer;
	SUser* m_pDistantPlayer;
	std::mutex m_oSyncMutex;
    std::mutex m_oStatMutex;

	std::chrono::time_point<system_clock> m_oSyncTransitionStart;

 public:
  CKernel(bool a_bIsServer);
  virtual ~CKernel();
  /// \returns the behavior tree
  CNode* GetBehaviorTree();

  CJsonParser* GetJsonParser();

  /// \brief Add the m_sSceneID to a_iPlayerID
  void AddSceneID(int a_iPlayerID, const std::string& m_sSceneID);

  /// \brief checks if the current player has this scene in his list
  bool PlayerHasScene(const std::string& a_sSceneID);

  int GetCurrentPlayer();

  void SendNetworkMessage(const std::string& a_rMessage);

  /// \brief send info about the local player to the remote player
  bool CheckPlayerInfo();


  /// \brief Initialize the kernel and the behavior tree
  void Init(const std::string& a_sPath);

  void WriteStats();


  /// \brief find the CEntityNode under the a_pTouch touch event and listening to a_rEvent
  /// \returns the found entity, nullptr otherwise
  CEntityNode* FindEntity(cocos2d::Touch* a_pTouch, const std::string& a_rEvent);
  void LogMessage(const std::string& a_sMessage);


  ////////////////// input callbacks
  /// \brief callback called when the user start touching the screen
  /// \details forward the event to the BehaviorTree in the kernel
  /// \returns true on success, false otherwise
  /// \param[in] a_pTouch the touch event
  /// \param[in] a_pEvent the cocos Event corresponding to this touch
  bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
  bool OnTouchEnd(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
  bool OnTouchMove(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

  ////////////////// json callbacks
  /// \brief go to the next scene in the tree
  void NavNext(cocos2d::Ref* pSender, CEntityNode* a_pTarget);
  /// \brief go to the previous scene in the tree
  void NavPrevious(cocos2d::Ref* pSender, CEntityNode* a_pTarget);

  void GotoScreenID(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief automatically validate the current scene
  void ValidateScene(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief Finds the Validator in the scene and validate the given ID
  /// \param[in] a_rEvent.m_sStringValue the id of the element to validate
  void Validate(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief change the visibility of the node
  /// \param[in] a_rEvent.m_bBoolValue is the chosen visibility
  void SetNodeVisible(SEvent a_rEvent, CEntityNode* a_pTarget);
  void SetNodeColored(SEvent a_rEvent, CEntityNode* a_pTarget);
  void FadeEntity(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief change the current player ID
  void SetPlayerID(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief change the current player name
  void SetPlayerName(SEvent a_rEvent, CEntityNode* a_pTarget);
  void SendNetworkMessage(SEvent a_rEvent, CEntityNode* a_pTarget);
  void LocalMessage(SEvent a_rEvent, CEntityNode* a_pTarget);
  void Connect(SEvent a_rEvent, CEntityNode* a_pTarget);
  void DisableEvent(SEvent a_rEvent, CEntityNode* a_pTarget);
  void EnableEvent(SEvent a_rEvent, CEntityNode* a_pTarget);
  void AnchorEntityCallback(SEvent a_rEvent, CEntityNode* a_pTarget);
  void PlaySoundCallback(SEvent a_rEvent, CEntityNode* a_pTarget);
  void SetText(SEvent a_rEvent, CEntityNode* a_pTarget);
  void RefreshPeers(SEvent a_rEvent, CEntityNode* a_pTarget);
  void EndGame(SEvent, CEntityNode*);
  void CountdownPressed(SEvent, CEntityNode*);
  void CountdownReleased(SEvent, CEntityNode*);
  void ValidateTeamTask(SEvent, CEntityNode*);

  //////////////// network callbacks
  void OnReceivingMessage(const std::string& a_rMessage);
  void OnReceiving(bytes a_rbyteArray, char a_cEventID);
  void GetPeers();
  void OnGettingPeers(const std::vector<std::string>& a_vPeers);


  void OnSoundEnded(const std::string& a_rSoundURL);

private:
	void AnchorEntity(CEntityNode* a_pAnchorEntity, CEntityNode* a_pAnchoredEntity);
	void ProcessMessage(const std::string& a_rMessage);
	void StartCountdownThread();
	void StopCountdownThread();

};


} // namespace LM

#endif /* _CKERNEL_H_ */
