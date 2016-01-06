#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"

using namespace cocos2d;

namespace LM
{

CKernel::CKernel() : m_oInputManager(*this)
{
  // the Behavior Tree is the root node of the tree
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

} // namespace LM
