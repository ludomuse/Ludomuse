#include "../Include/CGroupNode.h"


using namespace cocos2d;

namespace LM
{


CGroupNode::CGroupNode(EAnchor a_eAnchor,
                       int a_iWidth,
                       int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition,
	const std::string& a_rBackgroundSource) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
	m_sBackgroundSource(a_rBackgroundSource)
{
  
}

void CGroupNode::Init()
{

	if (m_sBackgroundSource == "")
	{
		m_pCocosEntity = Node::create();
	}
	else
	{
		m_pCocosEntity = Sprite::create(m_sBackgroundSource);
	}
  PopulateParent(false);

  Size oVisibleSize = GetParentVisibleSize();

  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);

  // TMP : background image
  if (m_sBackgroundSource != "")
  {
	  float fOldWidth = m_pCocosEntity->getBoundingBox().getMaxX() - m_pCocosEntity->getBoundingBox().getMinX();
	  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
	  float fNewWidthScale = fNewWidth / fOldWidth;

	  float fOldHeight = m_pCocosEntity->getBoundingBox().getMaxY() - m_pCocosEntity->getBoundingBox().getMinY();
	  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);
	  float fNewHeightScale = fNewHeight / fOldHeight;

	  m_pCocosEntity->setScale(fNewWidthScale, fNewHeightScale);
  }
  else
  {
	  m_pCocosEntity->setContentSize(Size(fNewWidth, fNewHeight));
  }

  CNode::Init();
}


} // namespace LM
