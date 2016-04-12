#ifndef _CCAMERAFEEDNODE_H_
#define _CCAMERAFEEDNODE_H_


#include "CEntityNode.h"

namespace LM
{

	typedef struct {
		GLfloat x;
		GLfloat y;
	} Vertex2D;

class CCameraGLView : public cocos2d::CCNode
{
	Vertex2D *vertices;
	float radius;
	int numSegments;

 public:
	 virtual bool init() override;
	 ~CCameraGLView();
  virtual void draw(cocos2d::Renderer* a_pRenderer,
                    const cocos2d::Mat4& a_rTransform,
                    uint32_t a_iFlags) override;
  
};


class CCameraFeedNode : public CEntityNode
{

 public:
	 CCameraFeedNode(EAnchor a_eAnchor, int width, int height, int x, int y);
  virtual void Init() override;
  
};

} // namespace LM

#endif /* _CCAMERAFEEDNODE_H_ */
