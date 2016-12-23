#include "functional"

#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CTransitionVisitor.h"
#include "../Include/CTouchBeganVisitor.h"
#include "../Include/CGotoSceneVisitor.h"
#include "../Include/CValidateSceneVisitor.h"
#include "../Include/CFindEntityVisitor.h"
#include "../Include/CDispatchEventVisitor.h"
#include "../Include/CDispatchMessageVisitor.h"
#include "../Include/CFindEntityFromIDVisitor.h"
#include "../Include/CFindEntityFromTypeVisitor.h"
#include "../Include/CEditorFindEntityTouchVisitor.h"
#include "../Include/CFindSceneFromIDVisitor.h"

#include "../Include/CInputManager.h"
#include "../Include/CSoundManager.h"
#include "../Include/CJsonParser.h"
#include "../../Modules/Networking/Networking.h"
#include "../../Modules/Util/Include/Util.h"
#include "../../Modules/Util/Include/CStats.h"

#include "ui/CocosGUI.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QMouseEvent>
#ifdef __ANDROID__
#include <GLES/gl.h>
#include "../../Modules/Networking/android/Include/LmJniJavaFacade.h"
#endif
#ifdef TARGET_OS_MAC
#include <unistd.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif




#define M_USER_EVENT '0'
#define M_STATS_EVENT '1'



using namespace cocos2d;

namespace LM
{

CKernel::CKernel(bool a_bIsServer) : m_pInputManager(new CInputManager(this)),
    m_pJsonParser(new CJsonParser(this)),
    m_bIsServer(a_bIsServer),
    //m_pNetworkManager(new CNetworkManager(this, a_bIsServer)),
    m_pSoundManager(new CSoundManager(this)),
    m_pBehaviorTree(new CSequenceNode()),
    m_bDebugMode(false),
    m_pLocalPlayer(new SUser()),
    m_pDistantPlayer(new SUser()),
    m_pDashboard(nullptr),
    m_pCurrentScene(nullptr),
    m_pWaitingScene(nullptr),
    m_pRemoteStats(nullptr)
{
    // the BehaviorTree member of the kernel
    // is a pointer to the root node of the tree

    // build the waiting scene
    // TODO : remove hardcoded waiting scene
    //    m_pWaitingScene = new CSceneNode("WaitingScene", this);
    //    CSpriteNode* pBackgroundSprite = new CSpriteNode("ui/waiting.png",
    //                                                     EAnchor::CENTER, 100, 100, 0, 0);

    //    m_pWaitingScene->AddChildNode(pBackgroundSprite);
}

CKernel::~CKernel()
{
    delete m_pBehaviorTree;
    delete m_pInputManager;
    //delete m_pNetworkManager;
    delete m_pSoundManager;
    delete m_pJsonParser;
    delete m_pLocalPlayer;
    delete m_pDistantPlayer;
}


CNode* CKernel::GetBehaviorTree()
{
    return m_pBehaviorTree;
}


CJsonParser* CKernel::GetJsonParser()
{
    return m_pJsonParser;
}

const std::vector<std::string> CKernel::GetSceneIDPlayer(int a_iPlayerID)
{
    if(a_iPlayerID == 0)
    {
        return this->m_mScenesID[0];
    }
    else if(a_iPlayerID == 1)
    {
        return this->m_mScenesID[1];
    }
    else
    {
        return this->m_mScenesID[0];
    }
}

std::string CKernel::ToJson(){

    rapidjson::StringBuffer s;

    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

    //    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    qDebug()<<"Lancement de la traduction vers Json";

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value app(rapidjson::kObjectType);
    app.AddMember("debug", true, allocator);
    rapidjson::Value colors(rapidjson::kArrayType);
    app.AddMember("colors", colors, allocator);
    rapidjson::Value images(rapidjson::kArrayType);
    app.AddMember("images", images, allocator);
    rapidjson::Value videos(rapidjson::kArrayType);
    app.AddMember("videos", videos, allocator);
    rapidjson::Value sounds(rapidjson::kArrayType);
    app.AddMember("sounds", sounds, allocator);
    rapidjson::Value scenes(rapidjson::kArrayType);
    this->ScenesToJson(scenes, allocator);
    app.AddMember("scenes", scenes, allocator);
    rapidjson::Value screens(rapidjson::kArrayType);
    if(m_pDashboard != nullptr)
    {
        m_pDashboard->ToJson(screens, allocator);
    }
    if (m_pWaitingScene != nullptr)
    {
        m_pWaitingScene->ToJson(screens, allocator);
    }
    this->ScreensToJson(screens, allocator);
    app.AddMember("screens", screens, allocator);

    document.AddMember("app", app, allocator);

    document.Accept(writer);
    //    qDebug()<<s.GetString();
    qDebug()<<"Fin de la traduction ----- Resultat";
    return s.GetString();
}



void CKernel::ScenesToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    std::vector<std::string>::iterator itSceneIDP1;
    std::vector<std::string>::iterator itSceneIDP2;
    // Assume that both vector have same length
    rapidjson::Value player1IDs(rapidjson::kArrayType);
    rapidjson::Value player2IDs(rapidjson::kArrayType);

    for (itSceneIDP1 = m_mScenesID[0].begin(); itSceneIDP1 != m_mScenesID[0].end(); ++itSceneIDP1)
    {
        if(itSceneIDP1->compare(""))
            player1IDs.PushBack(rapidjson::Value(itSceneIDP1->c_str(), itSceneIDP1->length()), a_rAllocator);
    }
    for (itSceneIDP2 = m_mScenesID[1].begin(); itSceneIDP2 != m_mScenesID[1].end(); ++itSceneIDP2)
    {
        if(itSceneIDP2->compare(""))
            player2IDs.PushBack(rapidjson::Value(itSceneIDP2->c_str(), itSceneIDP2->length()), a_rAllocator);
    }
    a_rParent.PushBack(player1IDs, a_rAllocator);
    a_rParent.PushBack(player2IDs, a_rAllocator);
}

void CKernel::ScreensToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    this->m_pBehaviorTree->ToJson(a_rParent, a_rAllocator);
}

bool CKernel::RemoveIDFromPlayer(const std::string &a_sSceneID, int a_iPlayerID)
{
    int index = find(this->m_mScenesID[a_iPlayerID].begin(), this->m_mScenesID[a_iPlayerID].end(), a_sSceneID) - this->m_mScenesID[a_iPlayerID].begin();
    if(index < m_mScenesID[a_iPlayerID].size())
    {
        this->m_mScenesID[a_iPlayerID].erase(this->m_mScenesID[a_iPlayerID].begin() + index);
        return true;
    }
    return false;
}

void CKernel::FullfillSyncedScenes()
{
    std::vector<std::string> syncScenesP1;
    std::vector<std::string> syncScenesP2;
    for(CNode* currentNode : m_pBehaviorTree->GetChildren())
    {
        CSceneNode* pSceneNode = dynamic_cast<CSceneNode*>(currentNode);
        if(pSceneNode)
        {
            if(pSceneNode->IsSynced())
            {
                if(this->PlayerHasScene(pSceneNode->GetSceneID(), 0))
                {
                    syncScenesP1.push_back(pSceneNode->GetSceneID());
                }
                else
                {
                    syncScenesP2.push_back(pSceneNode->GetSceneID());
                }
            }
        }
    }
    if(syncScenesP1.size() == syncScenesP2.size())
    {
        for(int i = 0; i < syncScenesP1.size(); i++)
        {
            this->m_mSceneSynced.emplace(syncScenesP1.at(i), syncScenesP2.at(i));
            this->m_mSceneSynced.emplace(syncScenesP2.at(i), syncScenesP1.at(i));
        }
    }
}

CEditorFindEntityTouchVisitor* CKernel::GetEditorVisitor()
{
    return this->m_oVisitor;
}

void CKernel::AddSceneID(int a_iPlayerID, const std::string& a_rSceneID)
{
    m_mScenesID[a_iPlayerID].push_back(a_rSceneID);
}

void CKernel::AddSceneIDAfter(int a_iPlayerID, const std::string& a_rSceneID, const std::string& a_rPreviousID)
{
    for(std::string currentString : m_mScenesID[a_iPlayerID])
    {
        if(currentString == a_rPreviousID)
        {
            int pos = find(m_mScenesID[a_iPlayerID].begin(), m_mScenesID[a_iPlayerID].end(), currentString) - m_mScenesID[a_iPlayerID].begin();
            //            qDebug()<<"Found id at index :"<<pos;
            m_mScenesID[a_iPlayerID].insert(m_mScenesID[a_iPlayerID].begin() + pos + 1, a_rSceneID);
            break;
        }
    }
}

void CKernel::AddSceneIDAtBegin(int a_iPlayerID, const std::string &a_sNewID)
{
    m_mScenesID[a_iPlayerID].insert(m_mScenesID[a_iPlayerID].begin(), a_sNewID);
}

void CKernel::AddNewScene(const std::string& a_sTemplatePath, const std::string& a_sPreviousID, const std::string& a_sNewID,
                          int a_iPlayerNumber, int a_iTemplateNumber, const std::string& a_sScreenMate)
{
    CSceneNode* newScene = new CSceneNode(a_sNewID, this);
    qDebug("ckernel add new scene");
    m_pJsonParser->BuildSceneNodeFromFile(newScene, a_sTemplatePath, a_iTemplateNumber, a_sScreenMate);

    // Adding id in the map
    // can add at   after an existing id of the player number
    //              at the end of only one player (the player number)
    switch(a_iPlayerNumber)
    {
    case 0: // Player 1 only
        if(std::find(m_mScenesID[0].begin(), m_mScenesID[0].end(), a_sPreviousID) != m_mScenesID[0].end())
        {
            this->AddSceneIDAfter(0, a_sNewID, a_sPreviousID);
        }
        else if(a_sPreviousID.empty())
        {
            this->AddSceneIDAtBegin(0, a_sNewID);
        }
        break;
    case 1: // Player 2 only
        if(std::find(m_mScenesID[1].begin(), m_mScenesID[1].end(), a_sPreviousID) != m_mScenesID[1].end())
        {
            this->AddSceneIDAfter(1, a_sNewID, a_sPreviousID);
        }
        else if(a_sPreviousID.empty())
        {
            this->AddSceneIDAtBegin(1,a_sNewID); // Add blank id at the other player timeline end
        }
        break;
    }

    // Adding the new scene at the right place
    if(a_sPreviousID.empty()) // Adding scene at the beginning
    {
        this->m_pBehaviorTree->AddChildNodeAtBegin(newScene);
        //        emit(addingSceneFinished(a_sNewID, a_iPlayerNumber));
        //        return;
    }
    else// Adding scene after an existing
    {
        this->m_pBehaviorTree->AddChildNodeAt(newScene, a_sPreviousID);
        //        emit(addingSceneFinished(a_sNewID, a_iPlayerNumber));
    }
    emit(addingSceneFinished(QString::fromStdString(a_sPreviousID),
                             QString::fromStdString(a_sNewID),
                             a_iPlayerNumber));
}

void CKernel::AddNewSharedScene(const std::string& a_sTemplatePath, const std::string& a_sPreviousID1, const std::string& a_sPreviousID2,
                                const std::string& a_sNewID, int a_iTemplateNumber, const std::string& a_sScreenMate)
{
    CSceneNode* newScene = new CSceneNode(a_sNewID, this);
    qDebug("ckernel add new scene");
    m_pJsonParser->BuildSceneNodeFromFile(newScene, a_sTemplatePath, a_iTemplateNumber, a_sScreenMate);
    // Adding id in the map
    // can add at   after an existing id of the player number
    //              at the end of only one player (the player number)
    if(a_sPreviousID1.empty())
    {
        this->AddSceneIDAtBegin(0, a_sNewID);
    }
    else if(std::find(m_mScenesID[0].begin(), m_mScenesID[0].end(), a_sPreviousID1) != m_mScenesID[0].end())
    {
        this->AddSceneIDAfter(0, a_sNewID, a_sPreviousID1);
    }
    if(a_sPreviousID2.empty())
    {
        this->AddSceneIDAtBegin(1, a_sNewID);
    }
    else if(std::find(m_mScenesID[1].begin(), m_mScenesID[1].end(), a_sPreviousID2) != m_mScenesID[1].end())
    {
        this->AddSceneIDAfter(1, a_sNewID, a_sPreviousID2); // Add blank id at the other player timeline end
    }

    // Adding the new scene at the right place in the behavior tree
    if(a_sPreviousID1.empty() && a_sPreviousID2.empty()) // Adding scene at the beginning
    {
        this->m_pBehaviorTree->AddChildNodeAtBegin(newScene);
    }
    else if (a_sPreviousID1.empty())// Adding scene after an existing
    {
        this->m_pBehaviorTree->AddChildNodeAt(newScene, a_sPreviousID2);
    }
    else if (a_sPreviousID2.empty())// Adding scene after an existing
    {
        this->m_pBehaviorTree->AddChildNodeAt(newScene, a_sPreviousID1);
    }
    else if (a_sPreviousID1 == a_sPreviousID2)
    {
        this->m_pBehaviorTree->AddChildNodeAt(newScene, a_sPreviousID1);
    }
    else
    {
        std::vector<CNode*> vChildren = this->m_pBehaviorTree->GetChildren();
        std::vector<CNode*>::iterator pPrevScene1;
        std::vector<CNode*>::iterator pPrevScene2;
        for (pPrevScene1 = vChildren.begin(); pPrevScene1 != vChildren.end(); pPrevScene1++)
        {
            if ((*pPrevScene1)->hasID(a_sPreviousID1)) {
                break;
            }
        }
        for (pPrevScene2 = vChildren.begin(); pPrevScene2 != vChildren.end(); pPrevScene2++)
        {
            if ((*pPrevScene2)->hasID(a_sPreviousID2)) {
                break;
            }
        }
        std::vector<CNode*>::iterator iBegin;
        std::vector<CNode*>::iterator iEnd;
        std::string sLastScene;
        int iPullPlayer;
        if (std::distance(pPrevScene1, pPrevScene2) > 0)
        {
            iEnd = pPrevScene2;
            iBegin = pPrevScene1;
            iPullPlayer = 1;
            sLastScene = a_sPreviousID2;
        }
        else
        {
            iEnd = pPrevScene1;
            iBegin = pPrevScene2;
            iPullPlayer = 0;
            sLastScene = a_sPreviousID1;
        }
        std::vector<CNode*>::iterator iFirst = iBegin + 1;
        std::vector<CNode*>::iterator iLast = iFirst + 1;
        while ((*iFirst)->GetSceneID() != sLastScene)
        {
            if (PlayerHasScene((*iFirst)->GetSceneID(), iPullPlayer))
            {
                iFirst++;
                iLast = iFirst + 1;
            }
            else if (iLast > iFirst && PlayerHasScene((*iLast)->GetSceneID(), iPullPlayer))
            {
                std::iter_swap(iLast-1, iLast);
                iLast --;
            }
            else
            {
                iLast++;
            }
        }
        newScene->SetParent(m_pBehaviorTree);
        vChildren.insert(iFirst, newScene);
        m_pBehaviorTree->SetChildren(vChildren);
    }
//    emit(addingSceneFinished(QString::fromStdString(a_sPreviousID),
//                             QString::fromStdString(a_sNewID),
//                             a_iPlayerNumber));
    emit(addingSharedSceneFinished(QString::fromStdString(a_sPreviousID1),
                             QString::fromStdString(a_sPreviousID2),
                             QString::fromStdString(a_sNewID)));
}

void CKernel::AddSyncID(const std::string& a_sID1, const std::string& a_sID2)
{
    this->m_mSceneSynced.emplace(a_sID1, a_sID2);
    this->m_mSceneSynced.emplace(a_sID2, a_sID1);
}

void CKernel::DeleteScene(const std::string &a_sSceneID)
{
    //Change current screen Trying to go to previous screen
    /*    CSceneNode* tempNode = m_pCurrentScene;
    CTransitionVisitor oVisitor(this, false);
    oVisitor.Traverse(m_pBehaviorTree);
    if(tempNode == m_pCurrentScene)
    {
        // Trying to go to the next screen
        qDebug("Premier screen");
        CTransitionVisitor oVisitor(this, true);
        oVisitor.Traverse(m_pBehaviorTree);
        if(tempNode == m_pCurrentScene)
        {
            qDebug("Dernier screen");
            return;
        }
    }*/
    this->m_pBehaviorTree->DeleteChildByID(a_sSceneID);
    //     Clear id from vector for both player
    //        this->RemoveIDFromPlayer(a_sSceneID, 0);
    //        this->RemoveIDFromPlayer(a_sSceneID, 1);
    bool bRemoved1 = this->RemoveIDFromPlayer(a_sSceneID, 0);
    bool bRemoved2 = this->RemoveIDFromPlayer(a_sSceneID, 1);
    emit deletingSceneFinished(QString::fromStdString(a_sSceneID));
    //        emit deletingSceneFinished();
}

void CKernel::DeleteSyncScenes(const std::string &a_sSceneID)
{
    // Change current screen by the previous or next
    /*    CSceneNode* tempNode = m_pCurrentScene;
    CTransitionVisitor oVisitor(this, false);
    oVisitor.Traverse(m_pBehaviorTree);
    if(tempNode == m_pCurrentScene)
    {
        // Trying to go to the next screen
        qDebug("Premier screen");
        CTransitionVisitor oVisitor(this, true);
        oVisitor.Traverse(m_pBehaviorTree);
        if(tempNode == m_pCurrentScene)
        {
            qDebug("Dernier screen");
            return;
        }
    }*/
    std::string otherID = m_mSceneSynced[a_sSceneID];
    // Removing scene from behavior, idlist and synced scene map
    /* this->m_pBehaviorTree->DeleteChildByID(a_sSceneID);
    this->m_pBehaviorTree->DeleteChildByID(otherID);
    if (this->RemoveIDFromPlayer(a_sSceneID, 0))
    {
        emit deletingSceneFinished(QString::fromStdString(a_sSceneID), 0);
    }
    if (this->RemoveIDFromPlayer(a_sSceneID, 1))
    {
        emit deletingSceneFinished(QString::fromStdString(a_sSceneID), 1);
    }
    if (this->RemoveIDFromPlayer(otherID, 0))
    {
        emit deletingSceneFinished(QString::fromStdString(otherID), 0);
    }
    if (this->RemoveIDFromPlayer(otherID, 1))
    {
        emit deletingSceneFinished(QString::fromStdString(otherID), 1);
    }*/
    DeleteScene(a_sSceneID);
    DeleteScene(otherID);
    //    this->RemoveIDFromPlayer(a_sSceneID, 0);
    //    this->RemoveIDFromPlayer(a_sSceneID, 1);
    //    this->RemoveIDFromPlayer(a_sSceneID, 0);
    //    this->RemoveIDFromPlayer(otherID, 1);
    this->m_mSceneSynced.erase(a_sSceneID);
    this->m_mSceneSynced.erase(otherID);
    //    emit deletingSceneFinished();
}

CSceneNode* CKernel::GetSyncedScene(CSceneNode* a_pScene)
{
    if (a_pScene->IsSynced())
    {
        std::string sSyncedScene = GetSyncedScene(m_pCurrentScene->GetID());
        return GetSceneNode(sSyncedScene);
    }
    return nullptr;
}

std::string CKernel::GetSyncedScene(const std::string &a_sSceneID)
{
    std::map<std::string,std::string>::iterator sFound;
    sFound = m_mSceneSynced.find(a_sSceneID);
    if (sFound != m_mSceneSynced.end())
    {
        return sFound->second;
    }
    else
    {
        return "";
    }
}

QString CKernel::GetSyncedScene(const QString& a_sSceneID)
{
    return QString::fromStdString(GetSyncedScene(a_sSceneID.toStdString()));
}

CSceneNode* CKernel::GetSceneNode(std::string a_sSceneID)
{
    return dynamic_cast<CSceneNode*>(m_pBehaviorTree->FindChildByID(a_sSceneID));
}

bool CKernel::PlayerHasScene(const std::string& a_rSceneID)
{
    return PlayerHasScene(a_rSceneID, m_pLocalPlayer->m_iPlayerID);
}

bool CKernel::PlayerHasScene(const std::string& a_rSceneID, int a_iPlayerID)
{
    std::vector<std::string>::iterator itSceneID;
    for (itSceneID = m_mScenesID[a_iPlayerID].begin();
         itSceneID != m_mScenesID[a_iPlayerID].end();
         ++itSceneID)
    {
        if (*itSceneID == a_rSceneID)
        {
            return true;
        }
    }
    return false;
}


int CKernel::GetCurrentPlayer()
{
    return m_pLocalPlayer->m_iPlayerID;
}

int CKernel::GetActivePlayer()
{
    if (PlayerHasScene(m_pCurrentScene->GetID(),1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void CKernel::SendNetworkMessage(const std::string& a_rMessage)
{
    //       m_pNetworkManager->Send(a_rMessage);
}



bool CKernel::CheckPlayerInfo()
{
    bytes b;
    b << M_USER_EVENT << *m_pLocalPlayer;
    CCLOG("local player : %d", m_pLocalPlayer->m_iPlayerID);
    //    m_pNetworkManager->Send(b);

    return m_pLocalPlayer->m_iPlayerID != m_pDistantPlayer->m_iPlayerID;
}


void CKernel::Init(const std::string& a_sPath)
{

    if(a_sPath.empty())
    {
        std::string sJsonPath = cocos2d::FileUtils::getInstance()->getStringFromFile("LudoMuse.conf");

        m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, sJsonPath);
    }
    else
    {
        m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, a_sPath);
    }

    CSceneNode* pFirstScene = (dynamic_cast<CSceneNode*>((*m_pBehaviorTree)[0]));
    m_pCurrentScene = pFirstScene;
    Scene* pScene = pFirstScene->CreateScene();
    pFirstScene->init();

    cocos2d::Director::getInstance()->runWithScene(pScene);
    M_STATS->StartStats();

    this->m_oVisitor = new CEditorFindEntityTouchVisitor(this);
    this->FullfillSyncedScenes();
    //M_STATS->PushStats("test");
    //CSerializableStats oSStats(M_STATS->GetStats());
    //WriteStats(&oSStats);

}


void CKernel::EndGame(SEvent, CEntityNode*)
{
    M_STATS->PushStats(m_pCurrentScene->GetSceneID());

    m_pLocalPlayer->m_bGameEnded = true;

    if (m_pLocalPlayer->m_iPlayerID == 1)
    {
        CSerializableStats oSStats(M_STATS->GetStats());
        bytes b;

        b << M_STATS_EVENT << oSStats;
        //		m_pNetworkManager->Send(b);

#ifdef __ANDROID__
        sleep(1);
#endif // __ANDROID__

        Director::getInstance()->end();

    }
    else if (m_pDistantPlayer->m_bGameEnded && m_pLocalPlayer->m_bGameEnded)
    {
        WriteStats();
    }

}


void CKernel::WriteStats()
{

    // TODO write stats to file on filesystem
    std::stringstream ss;
    ss << *m_pRemoteStats;
    CCLOG("[LUDO_STATS] ******************************************************************");
    CCLOG("[LUDO_STATS] remote peer : ");
    CCLOG("%s", ss.str().c_str());


    std::map<std::string, SScreenStats> mLocalStats = M_STATS->GetStats();
    std::map<std::string, SScreenStats> mRemoteStats = m_pRemoteStats->m_mScreensStats;

    std::stringstream fileStream;


    std::map<std::string, SScreenStats>::const_iterator itLocal;
    std::map<std::string, SScreenStats>::const_iterator itRemote;

    for (itLocal = mLocalStats.begin(), itRemote = mRemoteStats.begin();
         itLocal != mLocalStats.end() || itRemote != mRemoteStats.end();
         ++itLocal, ++itRemote)
    {

        const SScreenStats& rLocalStat = itLocal->second;
        const SScreenStats& rRemoteStat = itRemote->second;

        fileStream << rLocalStat.time << ","
                   << rLocalStat.nbInteractions << ","
                   << rLocalStat.nbTouches << ","
                   << rLocalStat.nbMoves << ","
                   << rLocalStat.nbValidTouches << ","
                   << rLocalStat.nbValidDrops << ","
                   << rLocalStat.nbInvalidDrops << ","
                   << rLocalStat.nbValidAnswers << ","
                   << rLocalStat.nbInvalidAnswers << ",";

        fileStream << ",";

        fileStream << rRemoteStat.time << ","
                   << rRemoteStat.nbInteractions << ","
                   << rRemoteStat.nbTouches << ","
                   << rRemoteStat.nbMoves << ","
                   << rRemoteStat.nbValidTouches << ","
                   << rRemoteStat.nbValidDrops << ","
                   << rRemoteStat.nbInvalidDrops << ","
                   << rRemoteStat.nbValidAnswers << ","
                   << rRemoteStat.nbInvalidAnswers << ",";

        fileStream << ",,";


        //fileStream << "Scene," << itLocal->first << "," << itRemote->first << std::endl;
        //fileStream << "Time," << rLocalStat.time  << "," << rRemoteStat.time << std::endl;
        //fileStream << "Interactions," << rLocalStat.nbInteractions << "," << rRemoteStat.nbInteractions << std::endl;
        //fileStream << "Touches," << rLocalStat.nbTouches << "," << rRemoteStat.nbTouches << std::endl;
        //fileStream << "Moves," << rLocalStat.nbMoves << "," << rRemoteStat.nbMoves << std::endl;
        //fileStream << "ValidTouches," << rLocalStat.nbValidTouches << "," << rRemoteStat.nbValidTouches << std::endl;
        //fileStream << "ValidDrops," << rLocalStat.nbValidDrops << "," << rRemoteStat.nbValidDrops << std::endl;
        //fileStream << "InvalidDrops," << rLocalStat.nbInvalidDrops << "," << rRemoteStat.nbInvalidDrops << std::endl;
        //fileStream << "ValidAnswers," << rLocalStat.nbValidAnswers << "," << rRemoteStat.nbValidAnswers << std::endl;
        //fileStream << "InvalidAnswers," << rLocalStat.nbInvalidAnswers << "," << rRemoteStat.nbInvalidAnswers << std::endl;

    }

    fileStream << std::endl;


    //std::string sPath = CCFileUtils::getInstance()->getWritablePath() + "stats.csv";
    //
    //CCLOG("[LUDO_STATS] trying to write stats to file : %s", sPath.c_str());

    //bool bWriteSuccess = CCFileUtils::getInstance()->writeStringToFile(fileStream.str(), sPath);

    //if (bWriteSuccess)
    //{
    //	CCLOG("[LUDO_STATS] Stats were successfully written to %s", sPath.c_str());
    //}
    //else
    //{
    //	CCLOG("[LUDO_STATS] Error while writing stats. Stats could not be written");
    //}

    //CCLOG("%s", fileStream.str().c_str());

    //FILE* statsFile = fopen(sPath.c_str(), "wb");
    //if (statsFile != NULL)
    //{
    //	const std::string sData = fileStream.str();
    //	const char* pData = sData.c_str();
    //	fputs(pData, statsFile);

    //	int iError = ferror(statsFile);
    //	if (iError == 0)
    //	{
    //		CCLOG("[LUDO_STATS] Stats were successfully written to %s", sPath.c_str());
    //	}
    //	else
    //	{
    //		CCLOG("[LUDO_STATS] stats could not be written to file. write failed with error code : %d", iError);
    //	}

    //}
    //else
    //{
    //	CCLOG("File could not be open");
    //}
    //fflush(statsFile);
    //fclose(statsFile);

    CCLOG("[LUDO_STATS] end WriteStats");

#ifdef __ANDROID__
    CCLOG("[LUDO_STATS] calling jni saveStringToFile");
    LmJniJavaFacade::saveStringToFile(fileStream.str());
#endif // __ANDROID__

#if defined _WIN32 | defined _WIN64
    Director::getInstance()->end();
#endif	// WINDOWS
}


void CKernel::NavNext(Ref* pSender, CEntityNode* a_pTarget)
{

    if (m_pCurrentScene->GetSceneID() == "screen-playerid")
    {
        if (!CheckPlayerInfo()) {
            // TODO throw a toast at the user

            return;
        }
    }

    CCMenuItemImage* pMenuItemImage = dynamic_cast<CCMenuItemImage*>(pSender);
    if (pMenuItemImage)
    {
        pMenuItemImage->setEnabled(false);
    }

    M_STATS->PushStats(m_pCurrentScene->GetSceneID());
    m_pSoundManager->PlaySound("ui/audio/buttonClicked.mp3");
    CDispatchMessageVisitor oMessageVisitor("Validated");
    oMessageVisitor.Traverse(m_pCurrentScene);
    CTransitionVisitor oVisitor(this, true);
    oVisitor.Traverse(m_pBehaviorTree);

    emit sendScene(m_pCurrentScene, true);
}

void CKernel::NavPrevious(Ref* pSender, CEntityNode* a_pTarget)
{

    CCMenuItemImage* pMenuItemImage = dynamic_cast<CCMenuItemImage*>(pSender);
    if (pMenuItemImage)
    {
        pMenuItemImage->setEnabled(false);
    }

    M_STATS->PushStats(m_pCurrentScene->GetSceneID());
    m_pSoundManager->PlaySound("ui/audio/buttonClicked.mp3");
    CTransitionVisitor oVisitor(this, false);
    oVisitor.Traverse(m_pBehaviorTree);

    emit sendScene(m_pCurrentScene, true);
}


bool CKernel::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
    M_STATS_SCREEN.nbInteractions++;
    //    CTouchBeganVisitor oVisistor(a_pTouch, a_pEvent, this);
    //    oVisistor.Traverse(m_pCurrentScene);

    //    m_mTouchBeganVisitors.insert(std::pair<int, CTouchBeganVisitor>(a_pTouch->getID(), oVisistor));
    qDebug("OnTouchBegan");
    //    this->m_oVisitor->SetEvent(a_pEvent);
    //    this->m_oVisitor->SetTouch(a_pTouch);
    this->m_oVisitor->SetVisitor(a_pEvent, a_pTouch);
    this->m_oVisitor->Traverse(m_pCurrentScene);
    return true;
}

bool CKernel::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
    std::map<int, CTouchBeganVisitor>::iterator itVisitor = m_mTouchBeganVisitors.find(a_pTouch->getID());
    if(itVisitor != m_mTouchBeganVisitors.end())
    {
        itVisitor->second.OnTouchEnd(a_pTouch, a_pEvent);
        m_mTouchBeganVisitors.erase(a_pTouch->getID());
    }

    return true;
}

bool CKernel::OnTouchMove(Touch* a_pTouch, Event* a_pEvent)
{
    std::map<int, CTouchBeganVisitor>::iterator itVisitor = m_mTouchBeganVisitors.find(a_pTouch->getID());
    if(itVisitor != m_mTouchBeganVisitors.end())
    {
        itVisitor->second.OnTouchMove(a_pTouch, a_pEvent);
    }

    return true;
}


void CKernel::GotoScreenID(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CCLOG("GotoScreenID : %s", a_oEvent.m_sStringValue.c_str());
    m_pLocalPlayer->m_iPlayerID = a_oEvent.m_iIntValue;
    if (a_oEvent.m_sStringValue == "") {
        ClearScreen();
    }
    else
    {
        CGotoSceneVisitor oVisitor(a_oEvent.m_sStringValue, this);
        oVisitor.Traverse(m_pBehaviorTree);
        emit sendScene(this->m_pCurrentScene, false);
    }
}

void CKernel::ClearScreen()
{
    m_pCurrentScene = /*new CSceneNode();*/ nullptr;
    Director::getInstance()->replaceScene(new Scene());
}

//void CKernel::GotoScreenID(const std::string &a_sSceneID, int a_iPlayerID)
//{
//    m_pLocalPlayer->m_iPlayerID = a_iPlayerID;
//    CGotoSceneVisitor oVisitor(a_sSceneID, this);
//    oVisitor.Traverse(m_pBehaviorTree);
//    qDebug() << QString::fromStdString("New scene : "+m_pCurrentScene->GetSceneID());

//    emit sendScene(this->m_pCurrentScene, false);
//}

void CKernel::CaptureScreen()
{
    //    cocos2d::utils::captureScreen(CC_CALLBACK_2(CKernel::afterCaptured, this),
    //                                  a_sFolder + m_pCurrentScene->GetSceneID()+".png");
    if (this->m_pCurrentScene != nullptr)
    {
        std::function<void(RenderTexture*, const std::string&)> callback =
                std::bind(&CKernel::ImageSaved, this, std::placeholders::_1, std::placeholders::_2);
        m_pCurrentScene->SaveImage(callback, 1);
    }
}

//void CKernel::CaptureScreenByID(SEvent a_oEvent, CEntityNode* a_pTarget)
//{
//    qDebug()<<"Save screenshot of %s" << a_oEvent.m_sStringValue.c_str();
//    m_pLocalPlayer->m_iPlayerID = a_oEvent.m_iIntValue;
//    CGotoSceneVisitor oVisitor(a_oEvent.m_sStringValue, this);
//    oVisitor.Traverse(m_pBehaviorTree);
//    std::string sFilePath = "";
//    sFilePath += a_oEvent.m_sStringValue;
//    sFilePath += ".png";
//    cocos2d::utils::captureScreen(CC_CALLBACK_2(CKernel::afterCaptured, this), sFilePath);
//}

//void CKernel::afterCaptured(bool succeed, const std::string& outputFile)
//{
//    if (succeed)
//    {
//        // show screenshot
//        qDebug("Capture screen succeed");
//        qDebug() << QString::fromStdString(outputFile);
//    }
//    else
//    {
//        qDebug("Capture screen failed.");
//    }
//}

void CKernel::ImageSaved(RenderTexture* a_pRender, const std::string& a_sOutputFile)
{
    std::string sSceneID = a_sOutputFile;
    const size_t last_slash_idx = sSceneID.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        sSceneID.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    const size_t period_idx = sSceneID.rfind('.');
    if (std::string::npos != period_idx)
    {
        sSceneID.erase(period_idx);
    }
    emit captureFinished(QString::fromStdString(sSceneID));
}

void CKernel::ValidateScene(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CCLOG("CKernel::ValidateScene");
    CValidateSceneVisitor oVisitor(a_oEvent);
    oVisitor.Traverse(m_pBehaviorTree);
    CCLOG("Scene validated successfully");
}

void CKernel::Validate(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    // find CValidator element in tree and validator->Validate(a_oEvent.m_sStringValue);
    Desc<CNode> pNode;
    CFindEntityFromTypeVisitor<CValidator> oVisitor(pNode);
    oVisitor.Traverse(m_pBehaviorTree);
    CCLOG("CKernel::Validate");
    if (pNode.IsValid())
    {
        CValidator* pValidator = static_cast<CValidator*>(pNode.Get());
        if (pValidator)
        {
            ON_CC_THREAD(CValidator::Validate, pValidator, a_oEvent.m_sStringValue);
        }
    }
}

void CKernel::SetNodeVisible(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CCLOG("CKernel::SetNodeVisible");
    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);
    if (pEntity)
    {
        pEntity->Show(a_oEvent.m_bBoolValue);
    }
}

void CKernel::FadeEntity(SEvent a_oEvent, CEntityNode* a_pTarget)
{

    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);

    if (pEntity)
    {
        pEntity->Fade();
    }
}

void CKernel::SetPlayerID(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    m_pLocalPlayer->m_iPlayerID = a_oEvent.m_iIntValue;
    CheckPlayerInfo();
}

void CKernel::SetPlayerName(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    Desc<CNode> pNode;
    CFindEntityFromIDVisitor oVisitor(pNode, a_rEvent.m_sStringValue);
    oVisitor.Traverse(m_pBehaviorTree);
    CEditBoxNode* pEntity = dynamic_cast<CEditBoxNode*>(pNode.Get());
    if (pEntity)
    {
        cocos2d::ui::EditBox* pBox = dynamic_cast<cocos2d::ui::EditBox*>(pEntity->GetCocosEntity());
        if (pBox)
        {
            m_pLocalPlayer->m_sName = pBox->getText();
            CCLOG("Player name set to %s", m_pLocalPlayer->m_sName.c_str());
        }
    }
}


CEntityNode* CKernel::FindEntity(Touch* a_pTouch, const std::string& a_sEvent)
{
    Desc<CNode> pEntity;
    CFindEntityTouchVisitor oVisitor(a_pTouch, pEntity, a_sEvent);
    oVisitor.Traverse(m_pBehaviorTree);
    return dynamic_cast<CEntityNode*>(pEntity.Get());
}


void CKernel::SendNetworkMessage(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CCLOG("Sending message %s", a_oEvent.m_sStringValue.c_str());
    //	m_pNetworkManager->Send(a_oEvent.m_sStringValue);
}

void CKernel::LocalMessage(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CCLOG("processing Local message %s", a_oEvent.m_sStringValue.c_str());
    ProcessMessage(a_oEvent.m_sStringValue);
}



void CKernel::ProcessMessage(const std::string& a_rMessage)
{
    std::vector<std::string> vSplittedMessage = StringSplit(a_rMessage);
    if (vSplittedMessage[0] == "kernel")
    {
        if (vSplittedMessage[1] == "waiting")
        {

            m_oSyncMutex.lock();

            if (m_pLocalPlayer->m_bWaiting)
            {
                std::chrono::milliseconds oTimeSinceTransitionStarted = duration_cast<milliseconds>(
                            std::chrono::system_clock::now() - m_oSyncTransitionStart);


                int iDelay = 600 - oTimeSinceTransitionStarted.count();

                if (iDelay > 0)
                {
#if defined __linux__ | defined TARGET_OS_MAC
                    usleep(iDelay * 1000);
#else
                    Sleep(iDelay);
#endif
                }

                ON_CC_THREAD(CKernel::NavNext, this, nullptr, nullptr);

            }
            else
            {
                m_pDistantPlayer->m_bWaiting = true;
            }

            m_oSyncMutex.unlock();
        }

        else if (vSplittedMessage[1] == "Validate")
        {
            ValidateScene(SEvent(), nullptr);
        }
    }
    else if (vSplittedMessage[0] == "Dashboard")
    {
        CDispatchMessageVisitor oVisitor(a_rMessage);
        ON_CC_THREAD(CDispatchMessageVisitor::Traverse, oVisitor, m_pDashboard);
    }
    else
    {
        CDispatchMessageVisitor oVisitor(a_rMessage);
        ON_CC_THREAD(CDispatchMessageVisitor::Traverse, oVisitor, m_pBehaviorTree);
    }
}

void CKernel::OnReceivingMessage(const std::string& a_rMessage)
{
    ProcessMessage(a_rMessage);
}

void CKernel::OnReceiving(bytes a_rByteArray, char a_cEventID)
{
    switch (a_cEventID)
    {
    case M_USER_EVENT:
        CCLOG("CKernel : On Receiving MSg");
        a_rByteArray >> &m_pDistantPlayer;
        CCLOG("CKernel : Distant player : %d", m_pDistantPlayer->m_iPlayerID);
        break;
    case M_STATS_EVENT:
        a_rByteArray >> &m_pRemoteStats;
        m_pDistantPlayer->m_bGameEnded = true;
        if (m_pLocalPlayer->m_bGameEnded && m_pDistantPlayer->m_bGameEnded)
        {
            WriteStats();
        }
        break;
    }
}

void CKernel::GetPeers()
{
    //	m_pNetworkManager->DiscoverPeers();
}

void CKernel::OnGettingPeers(const std::vector<std::string>& a_vPeers)
{
    CCLOG("peers : ");
    for (const std::string& itString : a_vPeers)
    {
        CCLOG("found peer : %s", itString.c_str());
    }
    Desc<CNode> pEntity;
    CFindEntityVisitor oVisitor(pEntity, "Peers");
    oVisitor.Traverse(m_pBehaviorTree);
    if (pEntity.IsValid())
    {
        CPeerNode* pPeerNode = static_cast<CPeerNode*>(pEntity.Get());
        if (pPeerNode)
        {
            ON_CC_THREAD(CPeerNode::AddPeers, pPeerNode, a_vPeers);
        }
    }
}

void CKernel::Connect(SEvent a_oEvent, CEntityNode* a_pTarget)
{
    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);
    if (pEntity)
    {
        Desc<CNode> pLabelDesc;
        CFindEntityVisitor oVisitor(pLabelDesc, "GetText");
        oVisitor.Traverse(pEntity);
        if (pLabelDesc.IsValid())
        {
            CEntityNode* pLabelEntity = dynamic_cast<CEntityNode*>(pLabelDesc.Get());
            if (pLabelEntity) {
                Label* pLabel = dynamic_cast<Label*>(pLabelEntity->GetCocosEntity());
                if (pLabel)
                {
                    //					m_pNetworkManager->ConnectTo(pLabel->getString());
                    //					m_pNetworkManager->Send("connection:establish");
                }
            }
        }
    }
}

void CKernel::DisableEvent(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    // string value should be build : "targetID:Event"
    std::vector<std::string> vArgs = StringSplit(a_rEvent.m_sStringValue);
    if (vArgs.size() > 1) {
        Desc<CNode> pNode;
        CFindEntityFromIDVisitor oVisitor(pNode, vArgs[0]);
        oVisitor.Traverse(m_pBehaviorTree);
        if (pNode.IsValid())
        {
            CEntityNode* pEntity = dynamic_cast<CEntityNode*>(pNode.Get());
            if (pEntity)
                pEntity->DisableEvent(vArgs[1]);
        }
    }
}


void CKernel::EnableEvent(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    std::vector<std::string> vArgs = StringSplit(a_rEvent.m_sStringValue);
    if (vArgs.size() > 1)
    {
        Desc<CNode> pNode;
        CFindEntityFromIDVisitor oVisitor(pNode, vArgs[0]);
        oVisitor.Traverse(m_pBehaviorTree);
        if (pNode.IsValid())
        {
            CEntityNode* pEntity = dynamic_cast<CEntityNode*>(pNode.Get());
            if (pEntity)
                pEntity->EnableEvent(vArgs[1]);
        }
    }
}


void CKernel::AnchorEntityCallback(SEvent a_rEvent, CEntityNode* a_pAnchoredEntity)
{
    std::string sExpectedID = a_rEvent.m_sStringValue;
    std::vector<std::string> vExpectedIDs = StringSplit(sExpectedID);
    std::vector<std::string>::iterator it;
    it = std::find(vExpectedIDs.begin(), vExpectedIDs.end(), a_pAnchoredEntity->GetID());
    CCLOG("AnchorEntity callback : %s", sExpectedID.c_str());
    CEntityNode* pAnchorEntity = dynamic_cast<CEntityNode*>(a_rEvent.m_pSender);
    if (pAnchorEntity)
    {
        CCLOG("pAnchoredEntity ID : %s", a_pAnchoredEntity->GetID().c_str());
        CCLOG("expected ID : %s", sExpectedID.c_str());
        if (a_pAnchoredEntity && it != vExpectedIDs.end())
        {
            AnchorEntity(pAnchorEntity, a_pAnchoredEntity);
            a_pAnchoredEntity->Dispatch("Anchored");
        }
        else {
            // put entity back
            CCLOG("put anchored entity back");
            auto dispatchMessage = CallFunc::create([a_pAnchoredEntity]() {
                CEntityNode::Release(a_pAnchoredEntity);
                a_pAnchoredEntity->Dispatch("AnchoredFailed");
            });

            auto revert = CallFunc::create([a_pAnchoredEntity]() {
                a_pAnchoredEntity->Revert();
            });

            auto oSequence = Sequence::create(dispatchMessage, revert, nullptr);

            a_pAnchoredEntity->GetCocosEntity()->runAction(oSequence);
        }
    }
}

void CKernel::AnchorEntity(CEntityNode* a_pAnchorEntity, CEntityNode* a_pAnchoredEntity)
{
    CCLOG("Tests passed");
    Node* pAnchor = a_pAnchorEntity->GetCocosEntity();
    Node* pAnchored = a_pAnchoredEntity->GetCocosEntity();

    Vec2 oAnchorPoint = pAnchor->getAnchorPoint();
    pAnchor->setAnchorPoint(Vec2(0.5f, 0.5f));
    Vec2 oLocation = pAnchor->getPosition();
    pAnchor->setAnchorPoint(oAnchorPoint);

    pAnchored->setAnchorPoint(Vec2(0.5f, 0.5f));
    pAnchored->setPosition(oLocation);


    float fOldWidth = pAnchored->getBoundingBox().getMaxX() - pAnchored->getBoundingBox().getMinX();
    float fNewWidth = pAnchor->getBoundingBox().getMaxX() - pAnchor->getBoundingBox().getMinX();
    float fNewScale = fNewWidth / fOldWidth;


    auto oScaleBy = ScaleBy::create(0.25, fNewScale);
    pAnchored->runAction(oScaleBy);


    for (CNode* itNode : *a_pAnchoredEntity)
    {
        CCLOG("Child node");
        CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
        if (pEntity)
        {
            AnchorEntity(a_pAnchorEntity, pEntity);
        }
    }

}



void CKernel::PlaySoundCallback(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    m_pSoundManager->PlaySound(a_rEvent.m_sStringValue);
}



void CKernel::OnSoundEnded(const std::string& a_rSoundURL)
{
    CDispatchEventVisitor oVisitor(std::string("SoundEnded:") + a_rSoundURL);
    oVisitor.Traverse(m_pCurrentScene);
}


void CKernel::SetText(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    CCLOG("CKernel::SetText");
    CLabelNode* pLabel = dynamic_cast<CLabelNode*>(a_rEvent.m_pSender);
    if (pLabel)
    {
        ON_CC_THREAD(CLabelNode::SetText, pLabel, a_rEvent.m_sStringValue);
    }
}


void CKernel::RefreshPeers(SEvent a_rEvent, CEntityNode* a_pTarget)
{
    //	m_pNetworkManager->DiscoverPeers();
}



void CKernel::LogMessage(const std::string& a_sMessage)
{
    CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
