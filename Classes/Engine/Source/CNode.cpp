#include "../Include/CNode.h"


namespace LM
{

CNode::CNode() : m_pCurrentNode(nullptr)
{
}

void CNode::AddChild(CNode* a_oChild)
{
  m_vChildren.push_back(a_oChild);
}


} // namespace LM
