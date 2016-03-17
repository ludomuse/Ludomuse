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
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition, m_sID)
{
  
}


void CEditBoxNode::Init()
{
  Size l_oVisibleSize = Director::getInstance()->getVisibleSize();
  Point l_oOrigin = Director::getInstance()->getVisibleOrigin();
  ui::EditBox* pBox = ui::EditBox::create(Size(1,
                                               l_oVisibleSize.height * 0.1),
                                          ui::Scale9Sprite::create("Ludomuse/Content/textfieldBackground.png"));
  pBox->setPlaceHolder("Nom de joueur");


  m_pCocosEntity = pBox;
  
  PopulateParent();

  CNode::Init();
}


}
