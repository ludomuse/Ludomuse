#include "../Include/CGroupNode.h"


using namespace cocos2d;

namespace LM
{


CGroupNode::CGroupNode(EAnchor a_eAnchor,
                       int a_iWidth,
                       int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition)
{
  
}

void CGroupNode::Init()
{
  m_pCocosEntity = Node::create();
  PopulateParent(false);

  Size oVisibleSize = GetParentVisibleSize();

  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);

  m_pCocosEntity->setContentSize(Size(fNewWidth, fNewHeight));


  CNode::Init();
}


} // namespace LM
