#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CEntityNode::CEntityNode(EAnchor a_eAnchor, int a_iXPosition, int a_iYPosition) :
    m_eAnchor(a_eAnchor),
	m_iXPosition(a_iXPosition),
    m_iYPosition(a_iYPosition)
{
}

cocos2d::Node* CEntityNode::GetCocosEntity()
{
	return m_pCocosEntity;
}


void CEntityNode::PopulateParent()
{

	Size oVisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 oOrigin = Director::getInstance()->getVisibleOrigin();
	
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


  cocos2d::Scene* pScene = ((CSceneNode*) m_pParent)->GetScene();
  if (pScene)
  {
    pScene->addChild(m_pCocosEntity, 0);
  }

}

} // namespace LM
