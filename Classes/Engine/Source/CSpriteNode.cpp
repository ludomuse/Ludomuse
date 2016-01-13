#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{


CSpriteNode::CSpriteNode(const std::string& a_rFilename,
                         int a_iXPosition,
                         int a_iYPosition) :
    CEntityNode(a_iXPosition, a_iYPosition),
    m_sSpriteFilename(a_rFilename)
{
  
}

void CSpriteNode::Init()
{
  auto m_pCocosEntity = Sprite::create(m_sSpriteFilename);
  m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));

  CSceneNode* pScene = (CSceneNode*) m_pParent;
  if (pScene)
  {
    pScene->addChild(m_pCocosEntity, 0);    
  }

}


} // namespace LM
