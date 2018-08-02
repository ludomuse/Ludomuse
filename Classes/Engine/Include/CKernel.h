#ifndef _CKERNEL_H_
#define _CKERNEL_H_

#ifdef LUDOMUSE_EDITOR
#include <QObject>
#include <QWidget>
#endif

#include "cocos2d.h"
#include "CSceneNode.h"
#include "SUser.h"
#include "../../Modules/Util/Include/CStats.h"

#ifdef LUDOMUSE_EDITOR
#include "ui/CocosGUI.h"

// Include for Json conversion
#include "rapidjson.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"

#endif
#include "SChapter.h"

#define ON_CC_THREAD(FUN, OBJ, ...) 	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(\
    std::bind(&FUN, OBJ, ##__VA_ARGS__));

namespace LM 
{

class CInputManager;
class CJsonParser;
class CNetworkManager;
class CSoundManager;
struct SEvent;
class CEntityNode;
class CTouchBeganVisitor;
#ifdef LUDOMUSE_EDITOR
class CEditorFindEntityTouchVisitor;
#endif

/// \class CKernel
/// \ingroup Engine
/// \brief the kernel of the game engine. Containing the behavior tree
#ifdef LUDOMUSE_EDITOR
class CKernel : public QObject
#else
class CKernel
#endif
{
#ifdef LUDOMUSE_EDITOR
    Q_OBJECT
#endif
private:

    /// \brief The parser that will build the behavior tree from the json file
    CJsonParser* m_pJsonParser;
#ifdef LUDOMUSE_EDITOR
    CEditorFindEntityTouchVisitor* m_oVisitor;
#endif

    std::map<int, std::vector<std::string> > m_mScenesID;
    
    std::vector<SChapter> mChapters;

    std::map<std::string, std::string> m_mSceneSynced;

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
  const std::vector<std::string> GetSceneIDPlayer(int a_iPlayerID);

#ifdef LUDOMUSE_EDITOR

    /// \brief Convert the whole game into Json
    std::string ToJson();

    CEditorFindEntityTouchVisitor* GetEditorVisitor();
#endif
  /// \brief Add the m_sSceneID to a_iPlayerID
  void AddSceneID(int a_iPlayerID, const std::string& m_sSceneID);

    /*CHAPTERSPROTOTYPE************************************************************************************************************************/
    void AddChapterScene(std::string chapterName,int playerId, std::string sceneName);
    void AddChapter(std::string chapterName, int chapterPosition);
    void DeleteChapter(std::string chapterName);
    int ChapterExist(std::string chapterName);
    void SeeChapters();
    std::string GetChapterName(int index);
    int GetChapterNumber();
    int GetNumberOfScene(int chapterNumber, int playerID);
    int GetSceneNumberCalculated(int chapterNumber);
    bool ChapterHasScene(int chapterNumber, std::string sceneName);
    void reorganizeChapters(int from , int to);
    /******************************************************************************************************************************************/


    /// \brief Add the m_sSceneID after the a_rPreviousID to a_iPlayerID
    /// \attention no test done to check if the id really exist in a_iPlayer scene ids !
    /// just does nothing if id doesn't exist in the list
    /// \param a_rPreviousID must be in a_iplayerID scene ids
    /// \param a_bInsertEmptyID if true, insert an empty id at the same
    void AddSceneIDAfter(int a_iPlayerID, const std::string& a_rSceneID, const std::string& a_rPreviousID, int chapterNumber);

    void AddSceneIDAtBegin(int a_iPlayer, const std::string& a_sNewID, int chapterNumber);

#ifdef LUDOMUSE_EDITOR
    /// \brief Add a whole new scene to the BehaviorTree
    /// \param previousID can be empty, if it's just add the new scene at the end of timeline
    void AddNewScene(const std::string& a_sTemplatePath, const std::string& previousID,const std::string& a_sNewID,
                     int a_iPlayerNumber, int chapterNumber, int a_iTemplateNumber = 0, const std::string& a_sScreenMateID = "");
    void AddScene(CSceneNode* newScene, const std::string& previousID,const std::string& a_sNewID,
                     int a_iPlayerNumber, int chapterNumber);
    void AddNewSharedScene(const std::string& a_sTemplatePath, const std::string& a_sPreviousID1, const std::string& a_sPreviousID2,
                           const std::string& a_sNewID, int a_iTemplateNumber, const std::string& a_sScreenMate, int chapterNumber);
#endif // LUDOMUSE_EDITOR


	void AddSyncID(const std::string& a_sID1, const std::string& a_sID2);

    /// \brief Delete scene matching id in arg
    void DeleteScene(const std::string& a_sSceneID);


    /// \brief Delete the sync scene and it's matching screen for the other player
    void DeleteSyncScenes(const std::string& a_sSceneID);

    /// \brief return the synced ID of a given scene (enpty string if the scene isnt synced)
    std::string GetSyncedScene(const std::string& a_sSceneID);
    #ifdef LUDOMUSE_EDITOR
    QString GetSyncedScene(const QString& a_sSceneID);
    #endif
    CSceneNode* GetSyncedScene(CSceneNode* a_pScene);

    /// \brief return the CSceneNode object which has a_sSceneID has ID
    CSceneNode* GetSceneNode(std::string a_sSceneID);

  /// \brief checks if the current player has this scene in his list
  bool PlayerHasScene(const std::string& a_sSceneID);

    /// \brief checks if the current player has this scene in his list
    bool PlayerHasScene(const std::string& a_sSceneID, int a_iPlayerID);

  int GetCurrentPlayer();
    int GetActivePlayer();

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
    void GotoDashboard();
    void GotoWaitingScene();
  void GotoScreenID(SEvent a_rEvent, CEntityNode* a_pTarget);
//    void GotoScreenID(const std::string& a_sSceneID, int a_iPlayerID);
    void ClearScreen();
    void ReloadScreen();

    /// \brief capture current screen
    void CaptureScreen();
    /// \brief go to scene and capture it!
    //    void CaptureScreenByID(SEvent a_rEvent, CEntityNode* a_pTarget);

    /// Callback called after capturing screen
    //    void afterCaptured(bool a_bSucceed, const std::string& outputFile);
    void ImageSaved(cocos2d::RenderTexture* render, const std::string& a_sOutputFile);

  /// \brief automatically validate the current scene
  void ValidateScene(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief Finds the Validator in the scene and validate the given ID
  /// \param[in] a_rEvent.m_sStringValue the id of the element to validate
  void Validate(SEvent a_rEvent, CEntityNode* a_pTarget);
  /// \brief change the visibility of the node
  /// \param[in] a_rEvent.m_bBoolValue is the chosen visibility
  void SetNodeVisible(SEvent a_rEvent, CEntityNode* a_pTarget);
    void SetNodeColored(SEvent a_oEvent, CEntityNode* a_pTarget);
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
	bool RemoveIDFromPlayer(const std::string& a_sSceneID, int a_iPlayerID = 0);

#ifdef LUDOMUSE_EDITOR
    void ScenesToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator,int chapterNumber);
    void ChaptersToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
    void ScreensToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
    void FullfillSyncedScenes();

signals:
    //    void addingSceneFinished(std::string a_sSceneID, int a_iPlayerID);
    //    void deletingSceneFinished();
    void addingSceneFinished(const QString a_sPrevSceneID, const QString a_sSceneID, int a_iPlayerID);
    void addingSharedSceneFinished(const QString a_sPrevSceneID1, const QString a_sPrevSceneID2, const QString a_sSceneID);
    void deletingSceneFinished(const QString a_sSceneID);
    void captureFinished(const QString a_sSceneID);
    /// \brief signal a new scene loaded, is Nav store the fact that it's navigation transition
    /// (with next and previous button) or if it's an transtition by id
    void sendScene(LM::CSceneNode*, bool a_bIsNav);
#endif
};


} // namespace LM

#endif /* _CKERNEL_H_ */
