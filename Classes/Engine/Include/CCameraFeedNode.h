#ifndef _CCAMERAFEEDNODE_H_
#define _CCAMERAFEEDNODE_H_


#include "CSpriteNode.h"

namespace LM
{

	typedef struct {
		GLfloat x;
		GLfloat y;
	} Vertex2D;

class CCameraGLView : public cocos2d::Sprite
{
	
	Vertex2D vertices[4];
	Vertex2D textCoord[4];

 public:
	 virtual bool init() override;
	 virtual void draw(cocos2d::Renderer* a_pRenderer,
                    const cocos2d::Mat4& a_rTransform,
                    uint32_t a_iFlags) override;
  
	 static CCameraGLView* create();
};


class CCameraFeedNode : public CSpriteNode
{
private:
	std::string m_sMaskPath;
	bool m_bIsReceiver;
  bool m_bIsInit;

 public:
	 CCameraFeedNode(const std::string& a_rMaskPath, EAnchor a_eAnchor, int width, int height, int x, int y, bool a_bIsReceiver = false);
  virtual void Init() override;
  virtual void UnInit(bool removeChild = true) override;

  void PictureTaken();

  void DisplayPicture(std::string);
  
};

} // namespace LM

#endif /* _CCAMERAFEEDNODE_H_ */
