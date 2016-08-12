#ifndef _CKERNEL_H_
#define _CKERNEL_H_



#include "cocos2d.h"
#include "CSceneNode.h"
#include "SUser.h"
#include "../../Modules/Util/Include/CStats.h"


#include "ui/CocosGUI.h"

#define ON_CC_THREAD(FUN, OBJ, ...) 	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(\
                                        std::bind(&FUN, OBJ, ##__VA_ARGS__));

// Include for Json conversion
#include "rapidjson.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"

#include <QObject>

namespace LM 
{

class CInputManager;
class CJsonParser;
class CNetworkManager;
class CSoundManager;
struct SEvent;
class CEntityNode;
class CTouchBeganVisitor;
class CEditorFindEntityTouchVisitor;

/// \class CKernel
/// \ingroup Engine
/// \brief the kernel of the game engine. Containing the behavior tree
class CKernel : public QObject
{
    Q_OBJECT
 private:
  /// \brief the Behavior Tree of the game
  /// \details a pointer to the root node of the tree, usually a SequenceNode
  CNode* m_pBehaviorTree;

  
  /// \brief The parser that will build the behavior tree from the json file
  CJsonParser* m_pJsonParser;

  CEditorFindEntityTouchVisitor* m_oVisitor;

  std::map<int, std::vector<std::string> > m_mScenesID;

  std::map<std::string, std::string> m_mSceneSynced;

  std::map<int, CTouchBeganVisitor> m_mTouchBeganVisitors;

  bool m_bIsServer;

  CSerializableStats* m_pRemoteStats;

public:
	/// \brief a reference to the dashboard with the timeline of the game
	CSceneNode* m_pDashboard;

	CSceneNode* m_pCurrentScene;

	CSceneNode* m_pWaitingScene;

	/// \details will forward the inputs to the behavior tree
	CInputManager* m_pInputManager;
	/// \details will manage networking events such as direct wifi
    //CNetworkManager* m_pNetworkManager;

	CSoundManager* m_pSoundManager;

	bool m_bDebugMode;

	SUser* m_pLocalPlayer;
	SUser* m_pDistantPlayer;
	std::mutex m_oSyncMutex;

	std::chrono::time_point<system_clock> m_oSyncTransitionStart;

public:
  CKernel(bool a_bIsServer);
  virtual ~CKernel();
  /// \returns the behavior tree
  CNode* GetBehaviorTree();

  CJsonParser* GetJsonParser();

  const std::vector<std::string> GetSceneIDPlayer(int a_iPlayerID);

  /// \brief Convert the whole game into Json
  std::string ToJson();

  CEditorFindEntityTouchVisitor* GetEditorVisitor();

  /// \brief Add the m_sSceneID to a_iPlayerID
  void AddSceneID(int a_iPlayerID, const std::string& m_sSceneID);

  /// \brief Add the m_sSceneID after the a_rPreviousID to a_iPlayerID
  /// \attention no test done to check if the id really exist in a_iPlayer scene ids !
  /// just does nothing if id doesn't exist in the list
  /// \param a_rPreviousID must be in a_iplayerID scene ids
  /// \param a_bInsertEmptyID if true, insert an empty id at the same
  void AddSceneIDAfter(int a_iPlayerID, const std::string& a_rSceneID, const std::string& a_rPreviousID);

  void AddSceneIDAtBegin(int a_iPlayer, const std::string& a_sNewID);

  /// \brief Add a whole new scene to the BehaviorTree
  /// \param previousID can be empty, if it's just add the new scene at the end of timeline
  void AddNewScene(const std::string& a_sTemplatePath, const std::string& previousID,const std::string& a_sNewID,
                   int a_iPlayerNumber, int a_iTemplateNumber = 0, const std::string& a_sScreenMateID = "");

  void AddSyncID(const std::string& a_sID1, const std::string& a_sID2);

  /// \brief Delete scene matching id in arg
  void DeleteScene(const std::string& a_sSceneID);


  /// \brief Delete the sync scene and it's matching screen for the other player
  void DeleteSyncScenes(const std::string& a_sSceneID);


  /// \brief checks if the current player has this scene in his list
  bool PlayerHasScene(const std::string& a_sSceneID);

  /// \brief checks if the current player has this scene in his list
  bool PlayerHasScene(const std::string& a_sSceneID, int a_iPlayerID);

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

  /// \brief go to scene and capture it!
  void CaptureScreenByID(SEvent a_rEvent, CEntityNode* a_pTarget);

  /// Callback called after capturing screen
  void afterCaptured(bool a_bSucceed, const std::string& outputFile);

  /// \brief automatically validate the current scene
  void ValidateScene(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief Finds the Validator in the scene and validate the given ID
  /// \param[in] a_rEvent.m_sStringValue the id of the element to validate
  void Validate(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief change the visibility of the node
  /// \param[in] a_rEvent.m_bBoolValue is the chosen visibility
  void SetNodeVisible(SEvent a_rEvent, CEntityNode* a_pTarget);
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


  //////////////// network callbacks
  void OnReceivingMessage(const std::string& a_rMessage);
  void OnReceiving(bytes a_rbyteArray, char a_cEventID);
  void GetPeers();
  void OnGettingPeers(const std::vector<std::string>& a_vPeers);


private:
	void AnchorEntity(CEntityNode* a_pAnchorEntity, CEntityNode* a_pAnchoredEntity);
    void ProcessMessage(const std::string& a_rMessage);
    void ScenesToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
    void ScreensToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
    void RemoveIDFromPlayer(const std::string& a_sSceneID, int a_iPlayerID = 0);
    void FullfillSyncedScenes();

signals:
    void addingSceneFinished();
    void deletingSceneFinished();

    /// \brief signal a new scene loaded, is Nav store the fact that it's navigation transition
    /// (with next and previous button) or if it's an transtition by id
    void sendScene(LM::CSceneNode*, bool a_bIsNav);
};


} // namespace LM

#endif /* _CKERNEL_H_ */
