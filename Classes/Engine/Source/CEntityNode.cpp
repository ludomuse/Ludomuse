#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CEntityNode::CEntityNode(EAnchor a_eAnchor, int a_iWidth, int a_iHeight,
						 int a_iXPosition, int a_iYPosition) :
    m_eAnchor(a_eAnchor),
	m_iXPosition(a_iXPosition),
    m_iYPosition(a_iYPosition),
	m_iWidth(a_iWidth),
	m_iHeight(a_iHeight)
{
}

cocos2d::Node* CEntityNode::GetCocosEntity()
{
	return m_pCocosEntity;
}


void CEntityNode::PopulateParent(bool a_bDoScaling)
{

	// Size oVisibleSize = Director::getInstance()->getVisibleSize();
	// Vec2 oOrigin = Director::getInstance()->getVisibleOrigin();
  Size oVisibleSize = GetParentVisibleSize();
  Vec2 oOrigin = GetParentOrigin();
	
	switch (m_eAnchor)
	{
	case LM::CENTER:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width/2, oOrigin.y + oVisibleSize.height/2));
		break;
	case LM::BOTTOM_LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y));
		break;
	case LM::LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y + oVisibleSize.height / 2));
		break;
	case LM::TOP_LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y + oVisibleSize.height));
		break;
	case LM::TOP:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width / 2, oOrigin.y + oVisibleSize.height));
		break;
	case LM::TOP_RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y + oVisibleSize.height));
		break;
	case LM::RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y + oVisibleSize.height / 2));
		break;
	case LM::BOTTOM_RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y));
		break;
	case LM::BOTTOM:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width / 2, oOrigin.y));
		break;
	case LM::FLOAT:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0.5));
		m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));
		break;
	default:
		break;
	}


	if (a_bDoScaling) {

		float fNewScale;

		if (m_iWidth != 0)
		{
			float fOldWidth = m_pCocosEntity->getBoundingBox().getMaxX() - m_pCocosEntity->getBoundingBox().getMinX();
			float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
			fNewScale = fNewWidth / fOldWidth;
		}
		else 
		{
			float fOldHeight = m_pCocosEntity->getBoundingBox().getMaxY() - m_pCocosEntity->getBoundingBox().getMinY();
			float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);
			fNewScale = fNewHeight / fOldHeight;
		}



		m_pCocosEntity->setScale(fNewScale);

	}
	cocos2d::Scene* pScene = GetParentScene();
	if (pScene)
	{
		pScene->addChild(m_pCocosEntity, 0);
	}

}


Scene* CEntityNode::GetParentScene()
{
	CSceneNode* pParentScene = dynamic_cast<CSceneNode*>(m_pParent);
	if (pParentScene)
	{
		return pParentScene->GetScene();
	}
	else
	{
		CEntityNode* pParentNode = dynamic_cast<CEntityNode*>(m_pParent);
		if (pParentNode)
		{
			return pParentNode->GetParentScene();
		}
	}
}

Size CEntityNode::GetParentVisibleSize()
{

  CEntityNode* pParentEntity = dynamic_cast<CEntityNode*>(m_pParent);
  if (pParentEntity)
  {
    return pParentEntity->GetParentVisibleSize();
  }
  
  return Director::getInstance()->getVisibleSize();
  
}

Vec2 CEntityNode::GetParentOrigin()
{
  CEntityNode* pParentEntity = dynamic_cast<CEntityNode*>(m_pParent);
  if (pParentEntity)
  {
    return pParentEntity->GetParentOrigin();
  }

  return Director::getInstance()->getVisibleOrigin();
}


Size CEntityNode::GetVsisibleSize()
{
	return m_pCocosEntity->getContentSize();
}

Vec2 CEntityNode::GetOrigin()
{
	Rect oBoundingBox = m_pCocosEntity->getBoundingBox();
	return Vec2(oBoundingBox.getMinX(), oBoundingBox.getMinY());
}

} // namespace LM
