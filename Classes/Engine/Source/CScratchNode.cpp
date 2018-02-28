#include "../Include/CScratchNode.h"
#include "../Include/CMacroManager.h"
#ifdef LUDOMUSE_EDITOR
#include "CProjectManager.h"
#endif

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
#ifndef LUDOMUSE_EDITOR
	  m_pDrawNode = DrawNode::create();
	  //m_pRenderTexture = RenderTexture::create(m_pCocosEntity->getBoundingBox().size.width, m_pCocosEntity->getBoundingBox().size.height);
	  //m_pRenderTexture->setPosition(m_pCocosEntity->getPosition().x, m_pCocosEntity->getPosition().y);
	  Vec2 minCorner = Vec2(
		  MAX(0, m_pCocosEntity->getBoundingBox().getMinX()),
		  MAX(0, m_pCocosEntity->getBoundingBox().getMinY())
	  );
	  Vec2 maxCorner = Vec2(
		  MIN(Director::getInstance()->getWinSize().width, m_pCocosEntity->getBoundingBox().getMaxX()),
		  MIN(Director::getInstance()->getWinSize().height, m_pCocosEntity->getBoundingBox().getMaxY())
		  );
	  m_pRenderTexture = RenderTexture::create(maxCorner.x - minCorner.x, maxCorner.y - minCorner.y);
	  m_pRenderTexture->setPosition((maxCorner.x + minCorner.x) / 2, (maxCorner.y + minCorner.y) / 2);
	  m_pDrawNode->setPosition(-minCorner);
	  m_pRenderTexture->retain();
	  m_pDrawNode->retain();
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
	
	  std::function<void(EventCustom*)> callback =
		  std::bind(&CScratchNode::DrawEvent, this, std::placeholders::_1);
	  //Director::getInstance()->getEventDispatcher()->addCustomEventListener("director_after_draw", callback);
	  // create a cocos EventListener and bind touchBegan to InputManager
	  auto pEventListener = EventListenerCustom::create("director_after_draw", callback);
	  auto pEventDispatcher = Director::getInstance()->getEventDispatcher();
	  pEventDispatcher->addEventListenerWithFixedPriority(pEventListener, 1);

	  m_bNodeDrawn = false;
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
#endif
  }


#ifdef LUDOMUSE_EDITOR
  void CScratchNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
  {
      // TODO
      rapidjson::Value spriteNode(rapidjson::kObjectType);
      spriteNode.AddMember("type", "Scratch", a_rAllocator);
      if(!m_sID.empty())
      {
          spriteNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
      }
      rapidjson::Value params(rapidjson::kObjectType);

      std::string temp = m_sSpriteFilename;
      std::string projectPath = CProjectManager::Instance()->GetProjectPath();
      int index = temp.find(projectPath);
      if(index != std::string::npos)
      {
          temp.erase(index, projectPath.length());
      }
      else
      {
          std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/templates/";
          int index2 = temp.find(templatePath);
          if(index2 != std::string::npos)
          {
              temp.erase(index2, templatePath.length());
          }
      }
      std::string* string = CProjectManager::Instance()->PushBackSource(temp);
      params.AddMember("source", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);

      params.AddMember("anchor", m_eAnchor, a_rAllocator);
      params.AddMember("width", m_iWidth, a_rAllocator);
      params.AddMember("height", m_iHeight, a_rAllocator);
      params.AddMember("x", m_iXPosition, a_rAllocator);
      params.AddMember("y", m_iYPosition, a_rAllocator);

      if(!this->m_mListeners.empty())
      {
          rapidjson::Value listeners(rapidjson::kArrayType);
          CEntityNode::ToJsonListener(listeners, a_rAllocator);
          params.AddMember("listeners", listeners, a_rAllocator);
      }

      if(!this->m_vChildren.empty())
      {
          rapidjson::Value children(rapidjson::kArrayType);
          for(CNode* currentNode : this->m_vChildren)
          {
              currentNode->ToJson(children, a_rAllocator);
          }
          params.AddMember("children", children, a_rAllocator);
      }

      spriteNode.AddMember("params", params, a_rAllocator);
      a_rParent.PushBack(spriteNode, a_rAllocator);
      /*{
              "type": "Image",
              "params": {
                "source": "cache/white.png",
                "anchor": 8,
                "width": 100,
                "height": 100,
                "x": 0,
                "y": 0
              }
            },
            */
  }
#endif


  void CScratchNode::DrawTouch(const Vec2& a_oTouchLocation)
  {
    //m_pDrawNode->drawDot(a_oTouchLocation - Vec2(100, 100), 200, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	  //Vec2 oLocalTouch = a_oTouchLocation - Vec2(m_pRenderTexture->getBoundingBox().getMinX(), m_pRenderTexture->getBoundingBox().getMinY());
	  if (m_bNodeDrawn == true) {
		  return;
	  }

	  //m_pDrawNode->drawDot(a_oTouchLocation, 40, Color4F(0.0f, 0.0f, 0.0f, 0.5f));
	  m_pDrawNode->drawSolidCircle(a_oTouchLocation, 40, 0, 24,Color4F(0.0f, 0.0f, 0.0f, 1.0f));
	  m_pRenderTexture->begin();
	  m_pDrawNode->visit();
	  //static_cast <Sprite*>(m_pCocosEntity)->visit();
	  m_pRenderTexture->end();
	  //Director::getInstance()->getRenderer()->render();
	  //m_pDrawNode->clear();
	  m_bNodeDrawn = true;
  }

  void CScratchNode::DrawEvent(EventCustom* ev) {
	  m_pDrawNode->clear();
	  m_bNodeDrawn = false;

  }
} // namespace LM
