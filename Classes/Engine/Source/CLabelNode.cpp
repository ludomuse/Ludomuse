#include "../Include/CLabelNode.h"

using namespace cocos2d;

namespace LM
{


CLabelNode::CLabelNode(const std::string& a_rText,
                       const std::string& a_rFontName,
                       int a_iFontSize,
					   const std::string& a_sTextAlign,
					   const std::string& a_sFontColor,
					   EAnchor a_eAnchor,
					   int a_iWidth,
					   int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sText(a_rText),
    m_sFontName(a_rFontName),
    m_iFontSize(a_iFontSize),
	m_sTextAlign(a_sTextAlign),
	m_sFontColor(a_sFontColor)
{
}


void CLabelNode::Init()
{
  Label* pLabel = Label::createWithTTF(m_sText, m_sFontName, m_iFontSize);
  m_pCocosEntity = pLabel;

  PopulateParent(false);
  
  // Size oVisibleSize = Director::getInstance()->getVisibleSize();
  Size oVisibleSize = GetParentVisibleSize();

  if (m_sTextAlign == "left")
  {
	  pLabel->setAlignment(TextHAlignment::LEFT);
  }
  else if (m_sTextAlign == "right")
  {
	  pLabel->setAlignment(TextHAlignment::RIGHT);
  }
  else
  {
	  pLabel->setAlignment(TextHAlignment::CENTER);
  }

  pLabel->setMaxLineWidth(oVisibleSize.width * (float)m_iWidth / 100.0f);


  std::vector<int> oRGBAValues;
  std::string sValue;
  std::istringstream oStream(m_sFontColor);

  while (std::getline(oStream, sValue, ','))
  {
	  int iValue = atoi(sValue.c_str());
	  oRGBAValues.push_back(iValue);
  }

  if (oRGBAValues.size() >= 4) // if color is not properly set, do nothing
  {
	  pLabel->setTextColor(Color4B(oRGBAValues[0], oRGBAValues[1], oRGBAValues[2], oRGBAValues[3]));
  }
  CNode::Init();
}


} // namespace LM
