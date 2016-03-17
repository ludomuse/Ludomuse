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
#include "../Include/CJsonParser.h"
#include "../../Modules/Networking/Networking.h"
#include "../../Modules/Util/Include/Util.h"


#include <fstream>


#define ON_CC_THREAD(FUN, OBJ, ...) 	Director::getInstance()->getScheduler()->performFunctionInCocosThread(\
										std::bind(&FUN, OBJ, ##__VA_ARGS__));


using namespace cocos2d;

namespace LM
{

CKernel::CKernel() : m_pInputManager(new CInputManager(this)), 
                     m_pJsonParser(new CJsonParser(this)),
                     m_pNetworkManager(new CNetworkManager(this)),
					 m_pBehaviorTree(new CSequenceNode()),
					 m_bCoopWaiting(false)
{
  // the BehaviorTree member of the kernel
  // is a pointer to the root node of the tree
	m_oLocalPlayer.m_iPlayerID = 1;
}

CKernel::~CKernel()
{
	delete m_pBehaviorTree;
	delete m_pInputManager;
	delete m_pNetworkManager;
	delete m_pJsonParser;
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
	for (itSceneID = m_mScenesID[m_oLocalPlayer.m_iPlayerID].begin();
			itSceneID != m_mScenesID[m_oLocalPlayer.m_iPlayerID].end();
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
	return m_oLocalPlayer.m_iPlayerID;
}

void CKernel::Init()
{
	std::string sJsonPath = cocos2d::FileUtils::getInstance()->getStringFromFile("LudoMuse.conf");

	m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, sJsonPath);

	CSceneNode* pFirstScene = (dynamic_cast<CSceneNode*>((*m_pBehaviorTree)[0]));

	Scene* pScene = pFirstScene->CreateScene();
	pFirstScene->init();

    cocos2d::Director::getInstance()->runWithScene(pScene);
}


void CKernel::NavNext(Ref* pSender, CEntityNode* a_pTarget)
{
	CTransitionVisitor oVisitor(this, true);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::NavPrevious(Ref* pSender, CEntityNode* a_pTarget)
{
	CTransitionVisitor oVisitor(this, false);
	oVisitor.Traverse(m_pBehaviorTree);
}


bool CKernel::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
	CTouchBeganVisitor oVisistor(a_pTouch, a_pEvent, this);
	oVisistor.Traverse(m_pBehaviorTree);

	EventListenerTouchOneByOne* pEventListener = m_pInputManager->GetEventListener();
	pEventListener->onTouchEnded = CC_CALLBACK_2(CTouchBeganVisitor::OnTouchEnd, oVisistor);
	pEventListener->onTouchMoved = CC_CALLBACK_2(CTouchBeganVisitor::OnTouchMove, oVisistor);

	return true;
}


void CKernel::GotoScreenID(CEvent a_oEvent, CEntityNode* a_pTarget)
{
	CCLOG("GotoScreenID : %s", a_oEvent.m_sStringValue.c_str());
	CGotoSceneVisitor oVisitor(a_oEvent.m_sStringValue);
	oVisitor.Traverse(m_pBehaviorTree);

}

void CKernel::ValidateScene(CEvent a_oEvent, CEntityNode* a_pTarget)
{
	CCLOG("CKernel::ValidateScene");
	CValidateSceneVisitor oVisitor(a_oEvent);
	oVisitor.Traverse(m_pBehaviorTree);
	CCLOG("Scene validated successfully");
}

void CKernel::Validate(CEvent a_oEvent, CEntityNode* a_pTarget)
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

void CKernel::SetNodeVisible(CEvent a_oEvent, CEntityNode* a_pTarget)
{
	CCLOG("CKernel::SetNodeVisible");
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);
	if (pEntity)
	{
		pEntity->Show(a_oEvent.m_bBoolValue);
	}
}

void CKernel::FadeEntity(CEvent a_oEvent, CEntityNode* a_pTarget)
{

	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);

	if (pEntity)
	{
		pEntity->Fade();
	}
}

void CKernel::SetPlayerID(CEvent a_oEvent, CEntityNode* a_pTarget)
{
	m_oLocalPlayer.m_iPlayerID = a_oEvent.m_iIntValue;
}


CEntityNode* CKernel::FindEntity(Touch* a_pTouch, const std::string& a_sEvent)
{
	Desc<CNode> pEntity;
	CFindEntityTouchVisitor oVisitor(a_pTouch, pEntity, a_sEvent);
	oVisitor.Traverse(m_pBehaviorTree);
	return dynamic_cast<CEntityNode*>(pEntity.Get());
}


void CKernel::SendNetworkMessage(CEvent a_oEvent, CEntityNode* a_pTarget)
{
	CCLOG("Sending message %s", a_oEvent.m_sStringValue.c_str());
	m_pNetworkManager->Send(a_oEvent.m_sStringValue);
}

void CKernel::OnReceivingMessage(const std::string& a_rMessage)
{
	std::string sKernel = "kernel";
	if (a_rMessage.substr(0, sKernel.size()) == sKernel)
	{
		std::vector<std::string> vSplittedMessage = StringSplit(a_rMessage);
		if (vSplittedMessage[1] == "waiting")
		{
			m_bCoopWaiting = true;
		}
	}
	else
	{
		CDispatchMessageVisitor oVisitor(a_rMessage);
		ON_CC_THREAD(CDispatchMessageVisitor::Traverse, oVisitor, m_pBehaviorTree);
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

void CKernel::Connect(CEvent a_oEvent, CEntityNode* a_pTarget)
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
					bytes b;
					b << m_oLocalPlayer;
					m_pNetworkManager->Send(b);
				}
			}
		}
	}
}

void CKernel::DisableEvent(CEvent a_rEvent, CEntityNode* a_pTarget)
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


void CKernel::EnableEvent(CEvent a_rEvent, CEntityNode* a_pTarget)
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


void CKernel::AnchorEntityCallback(CEvent a_rEvent, CEntityNode* a_pAnchoredEntity)
{
	std::string sExpectedID = a_rEvent.m_sStringValue;
	CCLOG("AnchorEntity callback : %s", sExpectedID.c_str());
	CNode* pAnchorNode = a_rEvent.m_pSender;
	CEntityNode* pAnchorEntity = dynamic_cast<CEntityNode*>(pAnchorNode);
	if (pAnchorEntity)
	{
		CCLOG("pAnchoredEntity ID : %s", a_pAnchoredEntity->GetID().c_str());
		CCLOG("expected ID : %s", sExpectedID.c_str());
		if (a_pAnchoredEntity && sExpectedID == a_pAnchoredEntity->GetID())
		{
			AnchorEntity(pAnchorEntity, a_pAnchoredEntity);
		}
		else {
			// put entity back
			CCLOG("put anchored entity back");
			a_pAnchoredEntity->Revert();
			CEntityNode::Release(a_pAnchoredEntity);
			a_pAnchoredEntity->Dispatch("AnchoredFailed");
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

	auto oScaleTo = ScaleTo::create(0.25, pAnchor->getScale());
	pAnchored->runAction(oScaleTo);

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

void CKernel::LogMessage(const std::string& a_sMessage)
{
	CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
