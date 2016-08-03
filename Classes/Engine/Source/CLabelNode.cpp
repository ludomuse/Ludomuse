#include "../Include/CLabelNode.h"
#include "../Include/CSpriteNode.h"

#include <QDebug>
#include <CProjectManager.h>

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


void CLabelNode::SetText(const std::string& a_rText)
{
	m_sText = a_rText;
	Label* pLabel = dynamic_cast<Label*>(m_pCocosEntity);
	if (pLabel)
	{
		pLabel->setString(m_sText);
	}
}

const std::string& CLabelNode::GetText() const
{
    return m_sText;
}

void CLabelNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    rapidjson::Value labelNode(rapidjson::kObjectType);
    labelNode.AddMember("type","Text",a_rAllocator);
    if(!m_sID.empty())
    {
        labelNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("content", rapidjson::Value(m_sText.c_str(), m_sText.length()),a_rAllocator);
    std::string temp = m_sFontName;
    std::string projectPath = CProjectManager::Instance()->GetProjectPath();
    int index = temp.find(projectPath);
    if(index != std::string::npos)
    {
        temp.erase(index, projectPath.length());
    }
    std::string* string = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("source", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);
    params.AddMember("fontSize", m_iFontSize, a_rAllocator);
    params.AddMember("textAlign", rapidjson::Value(m_sTextAlign.c_str(), m_sTextAlign.length()),a_rAllocator);
    params.AddMember("width", m_iWidth,a_rAllocator);
    params.AddMember("height", m_iHeight,a_rAllocator);
    params.AddMember("anchor", m_eAnchor,a_rAllocator);
    params.AddMember("color", rapidjson::Value(m_sFontColor.c_str(), m_sFontColor.length()),a_rAllocator);
    /* "type": "Text",
     "params": {
       "content": "SUIVANT",
       "font": "fonts/Open_Sans/OpenSans-Bold.ttf",
       "fontSize": 20,
       "textAlign": "right",
       "width": 80,
       "height": 100,
       "anchor": 0,
       "color": ""
     }*/

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

    labelNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(labelNode, a_rAllocator);
}

} // namespace LM
