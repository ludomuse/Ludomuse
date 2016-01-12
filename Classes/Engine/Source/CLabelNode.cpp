#include "../Include/CLabelNode.h"
#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{


CLabelNode::CLabelNode(const std::string& a_rText,
                       const std::string& a_rFontName,
                       int a_iFontSize,
                       int a_iXPosition,
                       int a_iYPosition) :
    CEntityNode(a_iXPosition, a_iYPosition),
    m_sText(a_rText),
    m_sFontName(a_rFontName),
    m_iFontSize(a_iFontSize)
{
}


void CLabelNode::Init()
{
  m_pLabel = Label::createWithTTF(m_sText, m_sFontName, m_iFontSize);
  m_pLabel->setPosition(Vec2(m_iXPosition, m_iYPosition));

  CSceneNode* pScene = (CSceneNode*) m_pParent;
  if (pScene)
  {
    pScene->addChild(m_pLabel, 0);    
  }
}

cocos2d::Label* CLabelNode::operator() ()
{
  return m_pLabel;
}

} // namespace LM
