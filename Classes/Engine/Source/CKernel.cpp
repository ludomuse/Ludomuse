#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"

#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{

CKernel::CKernel() : m_oInputManager(*this)
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
}


void CKernel::Init()
{

  // CJsonParser::BuildBehaviorTreeFromFile

  //CSceneNode oNode;
  //Scene* oScene = oNode.CreateScene();
  // node.init();
	m_oJsonParser.BuildBehaviorTreeFromFile(m_pBehaviorTree, "test.json");

	CSceneNode* pFirstScene = ((CSceneNode *)(*m_pBehaviorTree)[0]);

	Scene* oScene = pFirstScene->CreateScene();
	pFirstScene->init();

    cocos2d::Director::getInstance()->runWithScene(oScene);
  
}

} // namespace LM
