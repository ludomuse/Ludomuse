#include "../Include/CGroupNode.h"


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

  Size oVisibleSize = GetParentVisibleSize();

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
	  m_pCocosEntity->setContentSize(Size(fNewWidth, fNewHeight));
  }

  CNode::Init();
}

void CGroupNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{
    rapidjson::Value groupNode(rapidjson::kObjectType);
    groupNode.AddMember("type", "Group", a_rAllocator);
    rapidjson::Value params(rapidjson::kObjectType);
    if(!m_sBackgroundSource.empty())
    {
        params.AddMember("source", rapidjson::Value(m_sBackgroundSource.c_str(), m_sBackgroundSource.length()) , a_rAllocator);
    }
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

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
