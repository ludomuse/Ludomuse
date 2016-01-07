#include "../Include/CNode.h"


namespace LM
{

CNode::CNode() : m_iCurrentNode(-1)
{
}

CNode::~CNode()
{
}

void CNode::AddChild(CNode* a_oChild)
{
  m_vChildren.push_back(a_oChild);
}


} // namespace LM
