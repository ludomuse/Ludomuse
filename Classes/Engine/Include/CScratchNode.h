#ifndef _CSCRATCHNODE_H_
#define _CSCRATCHNODE_H_

#include "CSpriteNode.h"
#include <cocos2d.h>
#include <chrono>

namespace LM
{

  class CScratchNode : public CSpriteNode
  {
  private:
    //cocos2d::DrawNode* m_pDrawNode;

	// draw node defines where the sprite is revealed
	cocos2d::DrawNode* m_pDrawNode;
	// the render texture used to combine the draw node and the sprite
	cocos2d::RenderTexture* m_pRenderTexture;
    
	void DrawEvent(cocos2d::EventCustom* ev);

	bool m_bNodeDrawn;

  public:
    
    CScratchNode(const std::string& a_rSpriteFilename,
		 EAnchor a_eAnchor = EAnchor::FLOAT,
		 int a_iWidth = 0,
		 int a_iHeight = 0,
		 int a_iXPosition = 0,
		 int a_iYPosition = 0);

    virtual void Init() override;
    void DrawTouch(const cocos2d::Vec2& a_oTouchLocation);
  };


} // namespace LM



#endif // _CSCRATCHNODE_H_)
