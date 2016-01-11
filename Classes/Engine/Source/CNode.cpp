#include "../Include/CNode.h"


namespace LM
{

CNode::CNode() : m_iCurrentNode(-1), m_pParent(nullptr)
{
}

CNode::~CNode()
{
}

void CNode::AddChild(CNode* a_pChild)
{
  a_pChild->m_pParent = this;
  m_vChildren.push_back(a_pChild);
}


CNode::Iterator CNode::BeginChild()
{
  return m_vChildren.begin();
}

CNode::Iterator CNode::EndChild()
{
  return m_vChildren.end();
}


void CNode::Finish()
{
  m_pParent->Finish();
}

} // namespace LM
