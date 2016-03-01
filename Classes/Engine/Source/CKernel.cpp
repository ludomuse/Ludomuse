#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CTransitionVisitor.h"
#include "../Include/CTouchBeganVisitor.h"
#include "../Include/CGotoSceneVisitor.h"
#include "../Include/CValidateSceneVisitor.h"
#include "../Include/CFindEntityVisitor.h"

#include "../Include/CInputManager.h"
#include "../Include/CJsonParser.h"
#include "../../Modules/Networking/Networking.h"


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
					 m_iPlayerID(0)
{
  // the BehaviorTree member of the kernel
  // is a pointer to the root node of the tree
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
	for (itSceneID = m_mScenesID[m_iPlayerID].begin();
			itSceneID != m_mScenesID[m_iPlayerID].end();
			++itSceneID)
	{
		if (*itSceneID == a_rSceneID)
		{
			return true;
		}
	}

	return false;
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


void CKernel::NavNext(Ref* pSender)
{
	CTransitionVisitor oVisitor(this, true);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::NavPrevious(Ref* pSender)
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


void CKernel::GotoScreenID(CEvent a_rEvent)
{
	LogMessage("GotoScreenID : " + a_rEvent.m_sStringValue);
	CGotoSceneVisitor oVisitor(a_rEvent.m_sStringValue);
	oVisitor.Traverse(m_pBehaviorTree);

}

void CKernel::ValidateScene(CEvent a_rEvent)
{
	LogMessage("ValidateScene : " + a_rEvent.m_bBoolValue);
	CValidateSceneVisitor oVisitor(a_rEvent);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::SetNodeVisible(CEvent a_rEvent)
{
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_rEvent.m_pSender);
	if (pEntity)
	{
		pEntity->Show(a_rEvent.m_bBoolValue);
	}
}

void CKernel::SetPlayerID(CEvent a_rEvent)
{
	m_iPlayerID = a_rEvent.m_iIntValue;
}


CEntityNode* CKernel::FindEntity(Touch* a_pTouch, const std::string& a_sEvent)
{
	Desc<CEntityNode> pEntity;
	CFindEntityTouchVisitor oVisitor(a_pTouch, pEntity, a_sEvent);
	oVisitor.Traverse(m_pBehaviorTree);
	return pEntity.Get();
}


void CKernel::SendNetworkMessage(CEvent a_rEvent)
{
	CCLOG("Sending message %s", a_rEvent.m_sStringValue.c_str());
	m_pNetworkManager->Send(a_rEvent.m_sStringValue);
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
	Desc<CEntityNode> pEntity;
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

void CKernel::Connect(CEvent a_rEvent)
{
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_rEvent.m_pSender);
	if (pEntity)
	{
		Desc<CEntityNode> pLabelEntity;
		CFindEntityVisitor oVisitor(pLabelEntity, "GetText");
		oVisitor.Traverse(pEntity);
		if (pLabelEntity.IsValid())
		{
			Label* pLabel = dynamic_cast<Label*>(pLabelEntity.Get()->GetCocosEntity());
			if (pLabel)
			{
				m_pNetworkManager->ConnectTo(pLabel->getString());
				m_pNetworkManager->Send("connection:establish");
			}
		}
	}
}


void CKernel::LogMessage(const std::string& a_sMessage)
{
	CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
