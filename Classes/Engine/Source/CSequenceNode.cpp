#include "../Include/CSequenceNode.h"

using namespace cocos2d;


namespace LM
{

CSequenceNode::CSequenceNode() 
{
	m_iCurrentNode = 0;
}


bool CSequenceNode::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
  if (m_iCurrentNode != -1)
  {
    return m_vChildren[m_iCurrentNode]->OnTouchBegan(a_pTouch, a_pEvent);
  }

  return false;
}


void CSequenceNode::Finish()
{
  // increment the index of the current node in the sequence
  // if the current node was the last in the children, finish the sequence
  m_iCurrentNode++;
  if (m_iCurrentNode >= m_vChildren.size())
  {
    m_pParent->Finish();
  }
}


CNode* CSequenceNode::GetCurrentNode()
{
	return m_vChildren[m_iCurrentNode];
}

bool CSequenceNode::NextNode()
{
	if (m_iCurrentNode + 1 < m_vChildren.size())
	{
		m_iCurrentNode++;
		return true;
	}
	
	return false;
}

} // namespace LM
