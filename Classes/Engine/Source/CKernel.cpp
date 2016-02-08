#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CTransitionVisitor.h"

#include "../Include/CInputManager.h"
#include "../Include/CJsonParser.h"
#include "../../Modules/Networking/Networking.h"



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

  //CSceneNode oNode;
  //Scene* oScene = oNode.CreateScene();
  // node.init();
	m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, "Cern.json");

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


void CKernel::SendMessage(Ref* pSender)
{
	CCLOG("Sending message");
	m_pNetworkManager->Send("Hello from the other side !");
}

void CKernel::Connect(Ref* pSender)
{
	CCLOG("Hello World !");

	//m_pNetworkManager->Send("Hello World !");
	m_pNetworkManager->DiscoverPeers();
}

void CKernel::OnGettingPeers(std::vector<std::string> a_vPeers)
{
	CCLOG("connecting to : %s", a_vPeers[0].c_str());
	m_pNetworkManager->ConnectTo(a_vPeers[0]);
}


void CKernel::LogMessage(const std::string& a_sMessage)
{
	CCLOG("Kernel message : %s", a_sMessage.c_str());
}

} // namespace LM
