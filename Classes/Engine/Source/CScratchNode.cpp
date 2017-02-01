#include "../Include/CScratchNode.h"
#include "../Include/CMacroManager.h"

using namespace cocos2d;

namespace LM
{

  CScratchNode::CScratchNode(const std::string& a_rSpriteFilename,
			     EAnchor a_eAnchor,
			     int a_iWidth,
			     int a_iHeight,
			     int a_iXPosition,
			     int a_iYPosition) :
    CSpriteNode(a_rSpriteFilename,
		a_eAnchor,
		a_iWidth,
		a_iHeight,
		a_iXPosition,
		a_iYPosition)
  {
  }


  void CScratchNode::Init()
  {
	  CSpriteNode::Init();
	  m_pDrawNode = DrawNode::create();
	  m_pRenderTexture = RenderTexture::create(m_pCocosEntity->getBoundingBox().size.width, m_pCocosEntity->getBoundingBox().size.height);
	  //m_pRenderTexture->retain();
	  m_pDrawNode->retain();
	  m_pRenderTexture->setPosition(m_pCocosEntity->getPosition().x, m_pCocosEntity->getPosition().y);
	  //m_pRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	  cocos2d::Scene* pScene = GetParentScene();
	  if (pScene)
	  {
	      pScene->addChild(m_pRenderTexture, 0);
	  }
	  m_pRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 1.0f);
	  m_pRenderTexture->end();

	  BlendFunc bf;
	  // apply blending function to draw node
	  //bf.dst = GL_ONE_MINUS_SRC_ALPHA;
	  //bf.src = GL_SRC_ALPHA;
	  bf.dst = GL_ONE_MINUS_SRC_ALPHA;
	  bf.src = GL_ZERO;
	  m_pDrawNode->setBlendFunc(bf);
	  // apply blending function to sprite
	  //bf.src = GL_SRC_ALPHA;
	  //bf.dst = GL_ONE_MINUS_SRC_ALPHA;
	 //static_cast <Sprite*>(m_pCocosEntity)->setBlendFunc(bf);

	  // OLD CODE BELOW:
    
    //m_pDrawNode = DrawNode::create();
    //auto clippingNode = ClippingNode::create(m_pDrawNode);
    //auto sprite = Sprite::create(m_sSpriteFilename);
    //clippingNode->setAlphaThreshold(0.0f);
    //clippingNode->addChild(sprite);
    //m_pCocosEntity = sprite;

    //PopulateParent(true, false);
    //cocos2d::Scene* pScene = GetParentScene();
    //if (pScene)
    //{
    //    pScene->addChild(clippingNode, 0);
    //}


    //clippingNode->setContentSize(sprite->getBoundingBox().size);
    //clippingNode->setPosition(clippingNode->getPosition());

    //CNode::Init();
  }


  void CScratchNode::DrawTouch(const Vec2& a_oTouchLocation)
  {
    //m_pDrawNode->drawDot(a_oTouchLocation - Vec2(100, 100), 200, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	  Vec2 oLocalTouch = a_oTouchLocation - Vec2(m_pCocosEntity->getBoundingBox().getMinX(), m_pCocosEntity->getBoundingBox().getMinY());
	  m_pDrawNode->drawDot(oLocalTouch, 40, Color4F(0.0f, 0.0f, 0.0f, 0.5f));
	  m_pRenderTexture->begin();
	  m_pDrawNode->visit();
	  //static_cast <Sprite*>(m_pCocosEntity)->visit();
	  m_pRenderTexture->end();
	  Director::getInstance()->getRenderer()->render();
	  m_pDrawNode->clear();
  }

} // namespace LM
