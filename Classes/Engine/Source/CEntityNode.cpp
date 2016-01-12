#include "../Include/CEntityNode.h"

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

} // namespace LM
