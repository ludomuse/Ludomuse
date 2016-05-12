#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CTransitionVisitor.h"
#include "../Include/CTouchBeganVisitor.h"
#include "../Include/CGotoSceneVisitor.h"
#include "../Include/CValidateSceneVisitor.h"
#include "../Include/CFindEntityVisitor.h"
#include "../Include/CDispatchMessageVisitor.h"
#include "../Include/CFindEntityFromIDVisitor.h"
#include "../Include/CFindEntityFromTypeVisitor.h"

#include "../Include/CInputManager.h"
#include "../Include/CSoundManager.h"
#include "../Include/CJsonParser.h"
#include "../../Modules/Networking/Networking.h"
#include "../../Modules/Util/Include/Util.h"
#include "../../Modules/Util/Include/CStats.h"

#include "ui/CocosGUI.h"

#include <fstream>


#define ON_CC_THREAD(FUN, OBJ, ...) 	Director::getInstance()->getScheduler()->performFunctionInCocosThread(\
										std::bind(&FUN, OBJ, ##__VA_ARGS__));




#define M_USER_EVENT '0'
#define M_STATS_EVENT '1'



using namespace cocos2d;

namespace LM
{

	CKernel::CKernel() : m_pInputManager(new CInputManager(this)),
		m_pJsonParser(new CJsonParser(this)),
		m_pNetworkManager(new CNetworkManager(this)),
		m_pSoundManager(new CSoundManager(this)),
		m_pBehaviorTree(new CSequenceNode()),
		m_bDebugMode(false),
		m_bCoopWaiting(false),
		m_pLocalPlayer(new SUser()),
		m_pDistantPlayer(new SUser()),
		m_pDashboard(nullptr),
		m_pCurrentScene(nullptr)
{
  // the BehaviorTree member of the kernel
  // is a pointer to the root node of the tree
}

CKernel::~CKernel()
{
	delete m_pBehaviorTree;
	delete m_pInputManager;
	delete m_pNetworkManager;
	delete m_pSoundManager;
	delete m_pJsonParser;
	delete m_pLocalPlayer;
}


CNode* CKernel::GetBehaviorTree()
{
  return m_pBehaviorTree;
}


CJsonParser* CKernel::GetJsonParser()
{
	return m_pJsonParser;
}

void CKernel::AddSceneID(int a_iPlayerID, const std::string& a_rSceneID)
{
	m_mScenesID[a_iPlayerID].push_back(a_rSceneID);
}

bool CKernel::PlayerHasScene(const std::string& a_rSceneID)
{
	std::vector<std::string>::iterator itSceneID;
	for (itSceneID = m_mScenesID[m_pLocalPlayer->m_iPlayerID].begin();
			itSceneID != m_mScenesID[m_pLocalPlayer->m_iPlayerID].end();
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


void CKernel::SendNetworkMessage(const std::string& a_rMessage)
{
        m_pNetworkManager->Send(a_rMessage);
}



bool CKernel::CheckPlayerInfo()
{
	bytes b;
	b << M_USER_EVENT << *m_pLocalPlayer;
	CCLOG("local player : %d", m_pLocalPlayer->m_iPlayerID);
	m_pNetworkManager->Send(b);

	return m_pLocalPlayer->m_iPlayerID != m_pDistantPlayer->m_iPlayerID;
}


void CKernel::Init()
{
	std::string sJsonPath = cocos2d::FileUtils::getInstance()->getStringFromFile("LudoMuse.conf");

	m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, sJsonPath);

	CSceneNode* pFirstScene = (dynamic_cast<CSceneNode*>((*m_pBehaviorTree)[0]));
	m_pCurrentScene = pFirstScene;
	Scene* pScene = pFirstScene->CreateScene();
	pFirstScene->init();

    cocos2d::Director::getInstance()->runWithScene(pScene);

	M_STATS->StartStats();
}


void CKernel::EndGame(SEvent, CEntityNode*)
{
	if (m_pLocalPlayer->m_iPlayerID == 1)
	{
		CSerializableStats oSStats(M_STATS->GetStats());
		bytes b;

		b << M_STATS_EVENT << oSStats;
		m_pNetworkManager->Send(b);
	}
}


void CKernel::WriteStats(CSerializableStats* a_oSStats)
{
	// TODO write stats to file on filesystem
	std::stringstream ss;
	ss << a_oSStats;
	CCLOG("STATS remote peer : \n%s", ss.str().c_str());
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

	M_STATS->PushStats(m_pCurrentScene->GetSceneID());
  m_pSoundManager->PlaySound("ui/audio/buttonClicked.mp3");
	CDispatchMessageVisitor oMessageVisitor("Validated");
	oMessageVisitor.Traverse(m_pBehaviorTree);
	CTransitionVisitor oVisitor(this, true);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::NavPrevious(Ref* pSender, CEntityNode* a_pTarget)
{
	M_STATS->PushStats(m_pCurrentScene->GetSceneID());
  m_pSoundManager->PlaySound("ui/audio/buttonClicked.mp3");
  CTransitionVisitor oVisitor(this, false);
  oVisitor.Traverse(m_pBehaviorTree);
}


bool CKernel::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
	M_STATS_SCREEN.nbInteractions++;
	CTouchBeganVisitor oVisistor(a_pTouch, a_pEvent, this);
	oVisistor.Traverse(m_pCurrentScene);

	m_mTouchBeganVisitors.insert(std::pair<int, CTouchBeganVisitor>(a_pTouch->getID(), oVisistor));

	return true;
}

bool CKernel::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
	m_mTouchBeganVisitors.at(a_pTouch->getID()).OnTouchEnd(a_pTouch, a_pEvent);
	m_mTouchBeganVisitors.erase(a_pTouch->getID());

	return true;
}

bool CKernel::OnTouchMove(Touch* a_pTouch, Event* a_pEvent)
{
	m_mTouchBeganVisitors.at(a_pTouch->getID()).OnTouchMove(a_pTouch, a_pEvent);
	return true;
}


void CKernel::GotoScreenID(SEvent a_oEvent, CEntityNode* a_pTarget)
{
	CCLOG("GotoScreenID : %s", a_oEvent.m_sStringValue.c_str());
	CGotoSceneVisitor oVisitor(a_oEvent.m_sStringValue);
	oVisitor.Traverse(m_pBehaviorTree);

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
	m_pNetworkManager->Send(a_oEvent.m_sStringValue);
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
			m_bCoopWaiting = true;
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
		CSerializableStats* oSStats;
		a_rByteArray >> &oSStats;
		WriteStats(oSStats);
		break;
	}
}

void CKernel::GetPeers()
{
	m_pNetworkManager->DiscoverPeers();
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
					m_pNetworkManager->ConnectTo(pLabel->getString());
					m_pNetworkManager->Send("connection:establish");
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
	CCLOG("AnchorEntity callback : %s", sExpectedID.c_str());
	CEntityNode* pAnchorEntity = dynamic_cast<CEntityNode*>(a_rEvent.m_pSender);
	if (pAnchorEntity)
	{
		CCLOG("pAnchoredEntity ID : %s", a_pAnchoredEntity->GetID().c_str());
		CCLOG("expected ID : %s", sExpectedID.c_str());
		if (a_pAnchoredEntity && sExpectedID == a_pAnchoredEntity->GetID())
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
	m_pNetworkManager->DiscoverPeers();
}



void CKernel::LogMessage(const std::string& a_sMessage)
{
	CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
