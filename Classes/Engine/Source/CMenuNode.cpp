#include "../Include/CMenuNode.h"
#include "../Include/CLabelNode.h"//MEGAMERGE
#include "../Include/CSpriteNode.h"
#include "../Include/CKernel.h"
#ifdef LUDOMUSE_EDITOR
#include <CProjectManager.h>
#include <CEditorKernel.h>
#endif

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
#ifdef LUDOMUSE_EDITOR
    CCallback<CMenuNode, cocos2d::Ref*> callback("dummyNav", this, &CMenuNode::emitMenuNodeTouched);
    m_pMenuItemImage = MenuItemImage::create(
        m_sNormalImage,
        m_sSelectedImage,
        callback);
#else
  m_pMenuItemImage = MenuItemImage::create(
      m_sNormalImage,
      m_sSelectedImage,
      m_fpClickedCallback);
#endif

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
  cocos2d::Size oVisibleSize = GetParentVisibleSize();

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
  cocos2d::Rect oBoundingBox = m_pMenuItemImage->getBoundingBox();
  m_oSize = cocos2d::Size(oBoundingBox.getMaxX() - oBoundingBox.getMinX(),
	  oBoundingBox.getMaxY() - oBoundingBox.getMinY());
  m_oOrigin = Vec2(oBoundingBox.getMinX(), oBoundingBox.getMinY());

  CNode::Init();
}


cocos2d::Size CMenuNode::GetVisibleSize()
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

std::string CMenuNode::GetAction()
{
    return m_sAction;
}

std::string CMenuNode::GetText()
{
    CLabelNode* textNode = dynamic_cast<CLabelNode*>(m_vChildren.at(0));
    if(textNode)
    {
        return textNode->GetText();
    }
    else
    {
        return "";
    }
}

void CMenuNode::Show(bool a_bVisible)
{
	m_pCocosEntity->setVisible(true);
	CEntityNode::Show();
}
#ifdef LUDOMUSE_EDITOR
void CMenuNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    rapidjson::Value menu(rapidjson::kObjectType);
    menu.AddMember("type", "Nav", a_rAllocator);
    if(!m_sID.empty())
    {
        menu.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }
    // Create params properties
    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("width", this->m_iWidth, a_rAllocator);
    params.AddMember("height", this->m_iHeight, a_rAllocator);
    params.AddMember("anchor", this->m_eAnchor, a_rAllocator);

    std::string temp = m_sNormalImage;
    std::string projectPath = CProjectManager::Instance()->GetProjectPath();
    int index = temp.find(projectPath);
    if(index != std::string::npos)
    {
        temp.erase(index, projectPath.length());
    }
    else
    {
        std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/templates/";
        int index2 = temp.find(templatePath);
        if(index2 != std::string::npos)
        {
            temp.erase(index2, templatePath.length());
        }
    }
    std::string* string = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("normal", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);

    temp = m_sSelectedImage;
    index = temp.find(projectPath);
    if(index != std::string::npos)
    {
        temp.erase(index, projectPath.length());
    }
    else
    {
        std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/templates/";
        int index2 = temp.find(templatePath);
        if(index2 != std::string::npos)
        {
            temp.erase(index2, templatePath.length());
        }
    }
    std::string* string2 = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("selected", rapidjson::Value(string2->c_str(), string2->length()) , a_rAllocator);

    params.AddMember("action", rapidjson::Value(this->m_sAction.c_str(), this->m_sAction.length()), a_rAllocator);

    if(!this->m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if(!this->m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    menu.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(menu, a_rAllocator);
}

void CMenuNode::emitMenuNodeTouched (cocos2d::Ref* a_pDummy, CEntityNode* a_pDummy2)
{
    CEditorKernel::Instance()->sendMenuNode(this);
}
#endif
void CMenuNode::SetNavAction(bool a_bIsNext)
{
    if(a_bIsNext)
    {
        this->m_sAction = "next";
    }
    else
    {
        this->m_sAction = "prev";
    }
}

void CMenuNode::SetText(std::string a_sText)
{
    CLabelNode* textNode = dynamic_cast<CLabelNode*>(m_vChildren.at(0));
    if(textNode)
    {
        return textNode->SetText(a_sText);
    }
}
} // namespace LM
