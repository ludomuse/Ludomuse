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
  Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
  Point l_oOrigin = Director::getInstance()->getVisibleOrigin();
  ui::EditBox* pBox = ui::EditBox::create(Size(l_oVisibleSize.width/2.0f,
                                               l_oVisibleSize.height / 5.0f),
                                          ui::Scale9Sprite::create("Ludomuse/Content/textfieldBackground.png"));
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


}
