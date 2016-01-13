#include "../Include/CMenuNode.h"

using namespace cocos2d;

namespace LM
{

CMenuNode::CMenuNode(const std::string& a_rNormalImage,
                     const std::string& a_rSelectedImage,
                     FPMenuClickedCallback a_fpCallback,
                     int a_iXPosition,
                     int a_iYPosition) :
    CEntityNode(a_iXPosition, a_iYPosition),
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

  m_pMenuItemImage->setPosition(Vec2(m_iXPosition, m_iYPosition));

  m_pCocosEntity = Menu::create(m_pMenuItemImage, NULL);
  m_pCocosEntity->setPosition(Vec2::ZERO);

  PopulateParent();
}

} // namespace LM
