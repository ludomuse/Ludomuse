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


bool CNode::SetCurrentNode(CNode* a_pNode)
{
	for (int i = 0; i < m_vChildren.size(); ++i)
	{
		if (a_pNode == m_vChildren[i])
		{
			m_iCurrentNode = i;
			return true;
		}
	}
	return false;
}

std::vector<CNode*> CNode::GetChildren()
{
	return m_vChildren;
}

CNode* CNode::GetParent()
{
	return m_pParent;
}

void CNode::Init()
{
	for (CNode* pChildNode : m_vChildren) {
		pChildNode->Init();
	}
}

} // namespace LM
