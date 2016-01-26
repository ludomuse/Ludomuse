#include "../Include/CLabelNode.h"

using namespace cocos2d;

namespace LM
{


CLabelNode::CLabelNode(const std::string& a_rText,
                       const std::string& a_rFontName,
                       int a_iFontSize,
					   EAnchor a_eAnchor,
					   int a_iWidth,
					   int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sText(a_rText),
    m_sFontName(a_rFontName),
    m_iFontSize(a_iFontSize)
{
}


void CLabelNode::Init()
{
  m_pCocosEntity = Label::createWithTTF(m_sText, m_sFontName, m_iFontSize);

  PopulateParent();
  
}


} // namespace LM
