#include "../Include/CCameraFeedNode.h"
#include "../../Modules/Networking/android/Include/LmJniJavaFacade.h"

using namespace cocos2d;

namespace LM
{


	static inline Vertex2D Vertex2DMake(GLfloat inX, GLfloat inY) {
		Vertex2D ret;
		ret.x = inX;
		ret.y = inY;
		return ret;
	}



	bool CCameraGLView::init()
	{
		//bool success = CCNode::init();
		//if (success)
		//{
		//	radius = 100;
		//	numSegments = 10;

		//	vertices = (Vertex2D*)malloc(sizeof(Vertex2D) * (numSegments + 1 + 2));

		//	vertices[0] = Vertex2DMake(0, 0);

		//	float deltaAngle = 2 * M_PI / numSegments;

		//	for (int i = 1; i <= numSegments; i++) {
		//		GLfloat theta = deltaAngle*(i - 1);

		//		// Calculate x and y based on theta
		//		GLfloat x = radius * cosf(theta);
		//		GLfloat y = radius * sinf(theta);

		//		vertices[i] = Vertex2DMake(x, y);

		//	}

		//	vertices[numSegments + 1] = vertices[1];
		//}

		//return success;
	}

	CCameraGLView::~CCameraGLView()
	{
		//free(vertices);
	}


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
	//glPushMatrix();
	//glBegin(GL_TRIANGLES | GL_FILL);
	//glColor3f(0.1, 0.2, 0.3);
	//glVertex3f(0, 0, 0);
	//glVertex3f(1000, 0, 0);
	//glVertex3f(0, 1000, 0);	
	//glEnd();

	//CCNode::draw(a_pRenderer, a_rTransform, a_iFlags);

	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	//
	//glColor4f(1.0, 0.0, 0.0, 1.0);

	//glVertexPointer(2, GL_FLOAT, 0, vertices);

	//glDrawArrays(GL_TRIANGLE_FAN, 0, (numSegments + 2));

	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnable(GL_TEXTURE_2D);
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
  
	LmJniJavaFacade::takePicture();

  PopulateParent();
  
  CNode::Init();
}


} // namespace LM
