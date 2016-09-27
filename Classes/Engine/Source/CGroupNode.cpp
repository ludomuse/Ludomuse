#include "../Include/CGroupNode.h"

#include <CProjectManager.h>

using namespace cocos2d;

namespace LM
{


CGroupNode::CGroupNode(EAnchor a_eAnchor,
                       int a_iWidth,
                       int a_iHeight,
                       int a_iXPosition,
                       int a_iYPosition,
	const std::string& a_rBackgroundSource) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
	m_sBackgroundSource(a_rBackgroundSource)
{
  
}

void CGroupNode::Init()
{

	if (m_sBackgroundSource == "")
	{
		m_pCocosEntity = Node::create();
	}
	else
	{
		m_pCocosEntity = Sprite::create(m_sBackgroundSource);
	}
  PopulateParent(false);

  cocos2d::Size oVisibleSize = GetParentVisibleSize();

  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);

  // TMP : background image
  if (m_sBackgroundSource != "")
  {
	  float fOldWidth = m_pCocosEntity->getBoundingBox().getMaxX() - m_pCocosEntity->getBoundingBox().getMinX();
	  float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
	  float fNewWidthScale = fNewWidth / fOldWidth;

	  float fOldHeight = m_pCocosEntity->getBoundingBox().getMaxY() - m_pCocosEntity->getBoundingBox().getMinY();
	  float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);
	  float fNewHeightScale = fNewHeight / fOldHeight;

	  m_pCocosEntity->setScale(fNewWidthScale, fNewHeightScale);
  }
  else
  {
      m_pCocosEntity->setContentSize(cocos2d::Size(fNewWidth, fNewHeight));
  }

  CNode::Init();
}

void CGroupNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{
    rapidjson::Value groupNode(rapidjson::kObjectType);
    groupNode.AddMember("type", "Group", a_rAllocator);
    if(!m_sID.empty())
    {
        groupNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }
    rapidjson::Value params(rapidjson::kObjectType);
    if(!m_sBackgroundSource.empty())
    {
        std::string temp = m_sBackgroundSource;
        std::string projectPath = CProjectManager::Instance()->GetProjectPath();
        int index = temp.find(projectPath);
        if(index != std::string::npos)
        {
            temp.erase(index, projectPath.length());
        }
        else
        {
            std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/debug/templates/";
            int index2 = temp.find(templatePath);
            if(index2 != std::string::npos)
            {
                temp.erase(index2, templatePath.length());
            }
        }
        std::string* string = CProjectManager::Instance()->PushBackSource(temp);
        params.AddMember("source", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);
    }
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

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

    groupNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(groupNode, a_rAllocator);


}

} // namespace LM
