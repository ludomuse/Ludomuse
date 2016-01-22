#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"

#include "../Include/CSceneNode.h"

#include "../Include/CInputManager.h"
#include "../Include/CJsonParser.h"
#include "../Include/CTransitionVisitor.h"

using namespace cocos2d;

namespace LM
{

CKernel::CKernel() : m_pInputManager(new CInputManager(this)), 
				     m_pJsonParser(new CJsonParser(this))
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
  delete m_pJsonParser;
}


void CKernel::Init()
{

  //CSceneNode oNode;
  //Scene* oScene = oNode.CreateScene();
  // node.init();
	m_pJsonParser->BuildBehaviorTreeFromFile(m_pBehaviorTree, "test.json");

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

} // namespace LM
