#include "../Include/CEditBoxNode.h"


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
  pBox->setPlaceHolder("Nom de joueur");

  pBox->setFontSize(24);
  pBox->setFontName("fonts/Open_Sans/OpenSans-Bold.ttf");
  pBox->setFontColor(Color3B::BLACK);
  pBox->setMaxLength(80);
  pBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
  pBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

  m_pCocosEntity = pBox;
  
  PopulateParent();

  CNode::Init();
}


}
