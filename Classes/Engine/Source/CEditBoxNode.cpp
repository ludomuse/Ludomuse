#include "../Include/CEditBoxNode.h"
#include "../Include/CSpriteNode.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

namespace LM
{

CEditBoxNode::CEditBoxNode(EAnchor a_eAnchor,
                           int a_iWidth,
                           int a_iHeight,
                           int a_iXPosition,
                           int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition)
{
}


void CEditBoxNode::Init()
{
  cocos2d::Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
  cocos2d::Point l_oOrigin = Director::getInstance()->getVisibleOrigin();
  ui::EditBox* pBox = ui::EditBox::create(cocos2d::Size(l_oVisibleSize.width/2.0f,
                                               l_oVisibleSize.height / 5.0f),
                                          ui::Scale9Sprite::create("ui/textfieldBackground.png"));
  pBox->setFont("fonts/Open_Sans/OpenSans-Bold.ttf", 40);
  pBox->setFontColor(Color3B::BLACK);
  //pBox->setPlaceHolder("Nom de joueur");
  //pBox->setText("User");
  pBox->setMaxLength(16);
  pBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
  pBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

  m_pCocosEntity = pBox;
  
  PopulateParent();

  pBox->setPosition(pBox->getPosition() + Vec2(10, 0));

  CNode::Init();
}
#ifdef LUDOMUSE_EDITOR
void CEditBoxNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{
    rapidjson::Value editBoxNode(rapidjson::kObjectType);
    editBoxNode.AddMember("type", "EditBox", a_rAllocator);
    if(!m_sID.empty())
    {
        editBoxNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);

    rapidjson::Value children(rapidjson::kArrayType);
    if(!this->m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    editBoxNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(editBoxNode, a_rAllocator);
}
#endif

}
