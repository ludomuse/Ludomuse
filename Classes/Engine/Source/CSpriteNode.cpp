#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{


CSpriteNode::CSpriteNode(const std::string& a_rFilename,
	                     EAnchor a_eAnchor,
						 int a_iWidth,
						 int a_iHeight,
                         int a_iXPosition,
                         int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sSpriteFilename(a_rFilename)
{
  
}

void CSpriteNode::Init()
{
  m_pCocosEntity = Sprite::create(m_sSpriteFilename);
  if (!m_pCocosEntity)
  {
	  m_pCocosEntity = Sprite::create();
  }
  //m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));

  PopulateParent();

  CNode::Init();
}


void CSpriteNode::SetSpriteNode(const std::string& a_rNewPath)
{
	m_sSpriteFilename = a_rNewPath;
	CSceneNode* pSceneNode = GetParentSceneNode();
	pSceneNode->UnInit();
	pSceneNode->Init();
}

} // namespace LM
