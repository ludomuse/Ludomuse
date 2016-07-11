#include "../Include/CCameraFeedNode.h"
#include <fstream>
#include <string>

#ifdef __ANDROID__
#include <GLES/gl.h>
#include "../../Modules/Networking/android/Include/LmJniJavaFacade.h"
#include "../../Modules/Networking/android/Include/LmJniCppFacade.h"
#endif

using namespace cocos2d;

namespace LM
{


	static inline Vertex2D Vertex2DMake(GLfloat inX, GLfloat inY) {
		Vertex2D ret;
		ret.x = inX;
		ret.y = inY;
		return ret;
	}

	CCameraGLView* CCameraGLView::create()
	{
		CCameraGLView *pView = new (std::nothrow) CCameraGLView();
		if (pView && pView->init())
		{
			pView->autorelease();
			return pView;
		}
		CC_SAFE_DELETE(pView);
		return nullptr;
	}

	bool CCameraGLView::init()
	{
		bool success = Node::init();
		_texture = TextureCache::sharedTextureCache()->addImage("cache/AmuletteTigre.png");
		GLfloat size = _texture->getPixelsWide() / 2;

		vertices[0] = Vertex2DMake(-size, size);
		vertices[1] = Vertex2DMake(size, size);
		vertices[2] = Vertex2DMake(-size, -size);
		vertices[3] = Vertex2DMake(size, -size);

		textCoord[0] = Vertex2DMake(0, 0);
		textCoord[1] = Vertex2DMake(1, 0);
		textCoord[2] = Vertex2DMake(0, 1);
		textCoord[3] = Vertex2DMake(1, 1);

		return success;

		//if (success)
		//{
			//radius = 100;
			//numSegments = 10;

			//vertices = (Vertex2D*)malloc(sizeof(Vertex2D) * (numSegments + 1 + 2));

			//vertices[0] = Vertex2DMake(0, 0);

			//float deltaAngle = 2 * M_PI / numSegments;

			//for (int i = 1; i <= numSegments; i++) {
			//	GLfloat theta = deltaAngle*(i - 1);

			//	// Calculate x and y based on theta
			//	GLfloat x = radius * cosf(theta);
			//	GLfloat y = radius * sinf(theta);

			//	vertices[i] = Vertex2DMake(x, y);

			//}

		//}

		//return success;
		
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
	//Node::draw();
	//glPushMatrix();
	//int width = getContentSize().width;
	//int height = getContentSize().height;
	//int x = getBoundingBox().getMaxX() - getBoundingBox().getMinX();
	//int y = getBoundingBox().getMaxY() - getBoundingBox().getMinY();
	//glLineWidth(2.5);
	//glColor3f(1.0, 0.0, 0.0);
	//glBegin(GL_LINES);
	//glVertex3f(0.0, 0.0, 0.0);
	//glVertex3f(width, height, 0);
	//glVertex3f(x, y, 0);
	//glEnd();

	//Sprite::draw(a_pRenderer, a_rTransform, a_iFlags);
	//_trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, a_rTransform, a_iFlags);
	//a_pRenderer->addCommand(&_trianglesCommand);

	//Node::draw(a_pRenderer, a_rTransform, a_iFlags);

	/*glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisableClientState(GL_COLOR_ARRAY);
	




	//glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture->getName());

	//glColor4f(1.0, 0.0, 0.0, 1.0);

	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glTexCoordPointer(2, GL_FLOAT, 0, textCoord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glPopMatrix();





	glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnable(GL_TEXTURE_2D);

	//Texture2D* pTexture = getTexture();
	//CCLOG("texture name : %d", pTexture->getName());*/
}



///////////////////////////// class CCameraFeedNode


CCameraFeedNode::CCameraFeedNode(const std::string& a_rMaskPath, EAnchor a_eAnchor, int width, int height, int x, int y, bool a_bIsReceiver) :
	CSpriteNode("cache/bouddha1.png", a_eAnchor, width, height, x, y),
	m_bIsReceiver(a_bIsReceiver),
	m_sMaskPath(a_rMaskPath)
{
//	if (a_bIsReceiver)
//	{
//		CCLOG("LUDOMUSE - Construction CCameraFeedNode in receiver mode : %s", this->m_sID.c_str());
//		Director::getInstance()->getEventDispatcher()->addCustomEventListener("PictureReceived", CC_CALLBACK_0(CCameraFeedNode::PictureTaken, this));
//	}
//	else
//	{
//		CCLOG("LUDOMUSE - Construction CCameraFeedNode in sender mode : %s", this->m_sID.c_str());
//		Director::getInstance()->getEventDispatcher()->addCustomEventListener("PictureTaken", CC_CALLBACK_0(CCameraFeedNode::PictureTaken, this));
//	}
		
}

void CCameraFeedNode::Init()
{
	if (m_bIsReceiver)
	{
		CCLOG("LUDOMUSE - Construction CCameraFeedNode in receiver mode : %s", this->m_sID.c_str());
		Director::getInstance()->getEventDispatcher()->addCustomEventListener("PictureReceived", CC_CALLBACK_0(CCameraFeedNode::PictureTaken, this));
	}
	else
	{
		CCLOG("LUDOMUSE - Construction CCameraFeedNode in sender mode : %s", this->m_sID.c_str());
		Director::getInstance()->getEventDispatcher()->addCustomEventListener("PictureTaken", CC_CALLBACK_0(CCameraFeedNode::PictureTaken, this));
	}
#ifdef __ANDROID__
	if (!m_bIsReceiver)
	{
		LmJniJavaFacade::takePicture(m_sMaskPath);
	}
#endif
	CSpriteNode::Init();
}

void CCameraFeedNode::PictureTaken()
{
#ifdef __ANDROID__
	//UnInit();
	ON_CC_THREAD(CCameraFeedNode::DisplayPicture, this, LmJniCppFacade::getCurrentPicturePath()); 
#endif
}


void CCameraFeedNode::DisplayPicture(std::string sPicturePath) 
{
	CCLOG("LUDOMUSE - DisplayPicture %s begin", sPicturePath.c_str());
	m_pCocosEntity = Sprite::create(sPicturePath);

#ifdef __ANDROID__
	if (!m_bIsReceiver)
	{
		CCLOG("LUDOMUSE - Debut de l'envoie de fichier");
		LmJniJavaFacade::sendFile(sPicturePath);
		CCLOG("LUDOMUSE - Fin de l'envoie de fichier");
	}
#endif

	PopulateParent();
	CCLOG("LUDOMUSE - DisplayPicture ended");
}

} // namespace LM
