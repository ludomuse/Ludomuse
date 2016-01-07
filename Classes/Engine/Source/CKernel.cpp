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
  CSceneNode node;
  node.init();

  cocos2d::Director::getInstance()->runWithScene(node.GetScene());
  
}

} // namespace LM
