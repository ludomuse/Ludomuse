#include "../Include/CNode.h"


namespace LM
{

CNode::CNode() : m_iCurrentNode(0), m_pParent(nullptr)
{
}

CNode::~CNode()
{
	for (CNode* itChild : *this)
	{
		delete itChild;
	};
}



void CNode::AddChildNode(CNode* a_pChild)
{
  a_pChild->m_pParent = this;
  m_vChildren.push_back(a_pChild);
}


CNode::Iterator CNode::begin()
{
  return m_vChildren.begin();
}

CNode::Iterator CNode::end()
{
  return m_vChildren.end();
}


CNode* CNode::operator[](const int i)
{
	return m_vChildren[i];
}

/*void CNode::DoForEachChildNode(void(*a_fpCallback)(CNode* a_pChild))
{
	std::for_each(begin(), end(), a_fpCallback);
}*/


void CNode::Finish()
{
  m_pParent->Finish();
}

CNode* CNode::GetCurrentNode()
{
	return m_vChildren[m_iCurrentNode];
}

bool CNode::OffsetCurrentNode(int i)
{
	if (m_iCurrentNode + i < m_vChildren.size())
	{
		m_iCurrentNode += i;
		return true;
	}

	return false;
}

} // namespace LM
