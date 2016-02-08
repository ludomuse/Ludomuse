#include "../Include/CGridNode.h"


using namespace cocos2d;

namespace LM
{

CGridNode::CGridNode(EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition)
{
  
}


void CGridNode::Init()
{
	m_pCocosEntity = Node::create();
	PopulateParent();
	CNode::Init();
}


Size CGridNode::GetParentVisibleSize()
{
  return m_pCocosEntity->getContentSize();
}

Vec2 CGridNode::GetParentOrigin()
{
  Rect oBoundingBox = m_pCocosEntity->getBoundingBox();
  return Vec2(oBoundingBox.getMinX(), oBoundingBox.getMinY());
}

} // namespace LM
