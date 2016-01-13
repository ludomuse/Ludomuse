#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"

namespace LM
{

CEntityNode::CEntityNode(int a_iXPosition, int a_iYPosition) :
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
  CSceneNode* pScene = (CSceneNode*) m_pParent;
  if (pScene)
  {
    pScene->addChild(m_pCocosEntity, 0);
  }

}

} // namespace LM
