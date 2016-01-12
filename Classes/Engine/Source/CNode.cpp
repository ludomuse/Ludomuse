#include "../Include/CNode.h"


namespace LM
{

CNode::CNode() : m_iCurrentNode(-1), m_pParent(nullptr)
{
}

CNode::~CNode()
{
	DoForEachChildNode([](CNode* a_pChild) {
		delete a_pChild;
	});
}


bool CNode::OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent)
{
  return true;
}

void CNode::AddChildNode(CNode* a_pChild)
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

void CNode::DoForEachChildNode(void(*a_fpCallback)(CNode* a_pChild))
{
	std::for_each(BeginChild(), EndChild(), a_fpCallback);
}


void CNode::Finish()
{
  m_pParent->Finish();
}

} // namespace LM
