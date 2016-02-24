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

using namespace cocos2d;

namespace LM
{

CKernel::CKernel() : m_pInputManager(new CInputManager(this)), 
                     m_pJsonParser(new CJsonParser(this)),
                     m_pNetworkManager(new CNetworkManager(this))
{
  // the BehaviorTree member of the kernel
  // is a pointer to the root node of the tree
  m_pBehaviorTree = new CSequenceNode();
}


CNode* CKernel::GetBehaviorTree()
{
  return m_pBehaviorTree;
}


CKernel::~CKernel()
{
  delete m_pBehaviorTree;
  delete m_pInputManager;
  delete m_pNetworkManager;
  delete m_pJsonParser;
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
	CTransitionVisitor oVisitor(true);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::NavPrevious(Ref* pSender)
{
	CTransitionVisitor oVisitor(false);
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


void CKernel::GotoScreenID(CEvent a_oEvent)
{
	LogMessage("GotoScreenID : " + a_oEvent.m_sStringValue);
	CGotoSceneVisitor oVisitor(a_oEvent.m_sStringValue);
	oVisitor.Traverse(m_pBehaviorTree);

}

void CKernel::ValidateScene(CEvent a_oEvent)
{
	LogMessage("ValidateScene : " + a_oEvent.m_bBoolValue);
	CValidateSceneVisitor oVisitor(a_oEvent);
	oVisitor.Traverse(m_pBehaviorTree);
}

void CKernel::SetNodeVisible(CEvent a_oEvent)
{
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_oEvent.m_pSender);
	if (pEntity)
	{
		pEntity->Show(a_oEvent.m_bBoolValue);
	}
}

CEntityNode* CKernel::FindEntity(Touch* a_pTouch, const std::string& a_sEvent)
{
	CEntityNode* pEntity = nullptr;
	CFindEntityVisitor oVisitor(a_pTouch, &pEntity, a_sEvent);
	oVisitor.Traverse(m_pBehaviorTree);
	return pEntity;
}


void CKernel::SendMessage(Ref* pSender)
{
	CCLOG("Sending message");
	m_pNetworkManager->Send("Hello from the other side !");
}

void CKernel::GetPeers(Ref* pSender)
{
	CCLOG("Hello World !");

	//m_pNetworkManager->Send("Hello World !");
	m_pNetworkManager->DiscoverPeers();
}

void CKernel::OnGettingPeers(const std::vector<std::string>& a_vPeers)
{
	
}

void CKernel::Connect(const std::string& a_sPeer)
{
	//LogMessage("connecting to : %s", a_sPeer.c_str());
	m_pNetworkManager->ConnectTo(a_sPeer);
}


void CKernel::LogMessage(const std::string& a_sMessage)
{
	CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
