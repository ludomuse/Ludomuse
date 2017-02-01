#include <assert.h>
#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CMacroManager.h"


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
    Sprite* pSprite = Sprite::create(CMacroManager::Instance()->CheckDefinition(m_sSpriteFilename));
    //m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));

    if (!m_bColored)
      {
	BlendFunc oColoredBlend;
	oColoredBlend.src = GL_ZERO;
	oColoredBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
	pSprite->setBlendFunc(oColoredBlend);
      }
    //else
    //{
    // BlendFunc oColoredBlend;
    // oColoredBlend.src = GL_SRC_ALPHA;
    // oColoredBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
    // pBlendedNode->setBlendFunc(oColoredBlend);
    //}
    CCLOG("chargement de l'image : %s", m_sSpriteFilename.c_str());
    assert(pSprite && "Erreur : l'image n'a pas pu etre trouvée dans le dossier de scenario");
    m_pCocosEntity = pSprite;
    PopulateParent();

    CNode::Init();
  }


} // namespace LM
