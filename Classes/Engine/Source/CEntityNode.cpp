#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"

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
	// TODO place according to anchor
  m_pCocosEntity->setPosition(cocos2d::Vec2(m_iXPosition, m_iYPosition));

  CSceneNode* pScene = (CSceneNode*) m_pParent;
  if (pScene)
  {
    pScene->addChild(m_pCocosEntity, 0);
  }

}

} // namespace LM
