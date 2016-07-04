#include "../Include/CMenuNode.h"

using namespace cocos2d;

namespace LM
{

CMenuNode::CMenuNode(const std::string& a_rNormalImage,
                     const std::string& a_rSelectedImage,
                     CCallback<CKernel, cocos2d::Ref*> a_fpCallback,
					 EAnchor a_eAnchor,
					 int a_iWidth,
					 int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition,
                     const std::string& a_rAction) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sNormalImage(a_rNormalImage),
    m_sSelectedImage(a_rSelectedImage),
    m_fpClickedCallback(a_fpCallback),
    m_sAction(a_rAction)
{
}


void CMenuNode::Init()
{
  m_pMenuItemImage = MenuItemImage::create(
      m_sNormalImage,
      m_sSelectedImage,
      m_fpClickedCallback);

  m_pMenuItemImage->setPosition(Vec2::ZERO);

  m_pCocosEntity = Menu::create(m_pMenuItemImage, NULL);

  PopulateParent(false);

  // weird hack because cocos2d::Menu does not use its anchor point
  m_pMenuItemImage->setPosition(m_pCocosEntity->getPosition());
  m_pMenuItemImage->setAnchorPoint(m_pCocosEntity->getAnchorPoint());
  m_pMenuItemImage->setScale(m_pCocosEntity->getScale());
  
  m_pCocosEntity->setPosition(Vec2::ZERO);
  m_pCocosEntity->setScale(1);


  // Must be done in MenuItemImage for MenuNodes
  // Size oVisibleSize = Director::getInstance()->getVisibleSize();
  Size oVisibleSize = GetParentVisibleSize();

  float fNewScale;

  if (m_iWidth != 0)
  {
	  float fOldWidth = m_pMenuItemImage->getBoundingBox().getMaxX() - m_pMenuItemImage->getBoundingBox().getMinX();
	  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
	  fNewScale = fNewWidth / fOldWidth;
  }
  else
  {
	  float fOldHeight = m_pMenuItemImage->getBoundingBox().getMaxY() - m_pMenuItemImage->getBoundingBox().getMinY();
	  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);
	  fNewScale = fNewHeight / fOldHeight;
  }
  m_pMenuItemImage->setScale(fNewScale);


  // calculate the size and origin of the widget
  Rect oBoundingBox = m_pMenuItemImage->getBoundingBox();
  m_oSize = Size(oBoundingBox.getMaxX() - oBoundingBox.getMinX(),
	  oBoundingBox.getMaxY() - oBoundingBox.getMinY());
  m_oOrigin = Vec2(oBoundingBox.getMinX(), oBoundingBox.getMinY());

  CNode::Init();
}


Size CMenuNode::GetVisibleSize()
{
	return m_oSize;
}

Vec2 CMenuNode::GetOrigin()
{
	return m_oOrigin;
}

Node* CMenuNode::GetCocosEntity()
{
	return m_pMenuItemImage;
}

void CMenuNode::Show(bool a_bVisible)
{
	m_pCocosEntity->setVisible(true);
	CEntityNode::Show();
}

void CMenuNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    // TODO
    rapidjson::Value menu(rapidjson::kObjectType);
    menu.AddMember("type", "Nav", a_rAllocator);

    // Create params properties
    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("width", this->m_iWidth, a_rAllocator);
    params.AddMember("height", this->m_iHeight, a_rAllocator);
    params.AddMember("anchor", this->m_eAnchor, a_rAllocator);
    params.AddMember("normal", rapidjson::Value(this->m_sNormalImage.c_str(), this->m_sNormalImage.length()), a_rAllocator);
    params.AddMember("selected", rapidjson::Value(this->m_sSelectedImage.c_str(), this->m_sNormalImage.length()), a_rAllocator);
    params.AddMember("action", "next", a_rAllocator);
    /*"width": 0,
              "height": 13,
              "anchor": 5,
              "normal": "ui/nav-5.png",
              "selected": "ui/nav-5-active.png",
              "enabled": true,
              "color": "",
              "backgroundColor": "",
              "action": "next",*/
    menu.AddMember("params", params, a_rAllocator);

    a_rParent.PushBack(menu, a_rAllocator);
}

} // namespace LM
