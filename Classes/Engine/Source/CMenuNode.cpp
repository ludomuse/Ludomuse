#include "../Include/CMenuNode.h"

using namespace cocos2d;

namespace LM
{

CMenuNode::CMenuNode(const std::string& a_rNormalImage,
                     const std::string& a_rSelectedImage,
                     CCallback a_fpCallback,
					 EAnchor a_eAnchor,
					 int a_iWidth,
					 int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sNormalImage(a_rNormalImage),
    m_sSelectedImage(a_rSelectedImage),
    m_fpClickedCallback(a_fpCallback)
{
}


void CMenuNode::Init()
{
  auto m_pMenuItemImage = MenuItemImage::create(
      m_sNormalImage,
      m_sSelectedImage,
      m_fpClickedCallback);

  m_pMenuItemImage->setPosition(Vec2::ZERO);

  m_pCocosEntity = Menu::create(m_pMenuItemImage, NULL);

  PopulateParent(false);

  // weird hack because cocos2d::Menu does not use its anchor point
  m_pMenuItemImage->setPosition(m_pCocosEntity->getPosition());
  m_pMenuItemImage->setAnchorPoint(m_pCocosEntity->getAnchorPoint());
  m_pMenuItemImage->setScale(m_pCocosEntity->getScaleX(), m_pCocosEntity->getScaleY());
  
  m_pCocosEntity->setPosition(Vec2::ZERO);
  m_pCocosEntity->setScale(1);


  // Must be done in MenuItemImage for MenuNodes
  Size oVisibleSize = Director::getInstance()->getVisibleSize();
  float iOldWidth = m_pMenuItemImage->getBoundingBox().getMaxX() - m_pMenuItemImage->getBoundingBox().getMinX();
  float iOldHeight = m_pMenuItemImage->getBoundingBox().getMaxY() - m_pMenuItemImage->getBoundingBox().getMinY();

  float iNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
  float iNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);

  float iScaleX = iNewWidth / iOldWidth;
  float iScaleY = iNewHeight / iOldHeight;

  m_pMenuItemImage->setScale(iScaleX, iScaleY);

  CNode::Init();
}

} // namespace LM
