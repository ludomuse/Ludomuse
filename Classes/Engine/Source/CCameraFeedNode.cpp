#include "../Include/CCameraFeedNode.h"


using namespace cocos2d;

namespace LM
{


/////////////////////////// class CCameraGLView

void CCameraGLView::draw(Renderer* a_pRenderer,
                         const Mat4& a_rTransform,
                         uint32_t a_iFlags)
{
	//a_pRenderer->clear();
	//ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
	//
	//glEnableVertexAttribArray(kCCVertexAttribFlag_Position);
	//glVertexAttribPointer(kCCVertexAttribFlag_Position, 3, GL_FLOAT, GL_FALSE, 0, dynamicVerts);

	//glEnableVertexAttribArray(kCCVertexAttribFlag_Color);
	//glVertexAttribPointer(kCCVertexAttribFlag_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, dynamicvertColors);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, dynamicVertCount);
	glPushMatrix();
	glBegin(GL_TRIANGLES | GL_FILL);
	glColor3f(0.1, 0.2, 0.3);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glVertex3f(0, 1000, 0);	
	glEnd();
}



///////////////////////////// class CCameraFeedNode


CCameraFeedNode::CCameraFeedNode(EAnchor a_eAnchor, int width, int height, int x, int y) :
	CEntityNode(a_eAnchor, width, height, x, y)
{

}

void CCameraFeedNode::Init()
{
	CCameraGLView* g = new CCameraGLView();
	m_pCocosEntity = g;
  
  PopulateParent();
  
  CNode::Init();
}


} // namespace LM
