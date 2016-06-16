#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"

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
  m_pCocosEntity = Sprite::create(m_sSpriteFilename);
  //m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));

  PopulateParent();

  CNode::Init();
}


const std::string& CSpriteNode::GetPath() const
{
    return this->m_sSpriteFilename;
}

void CSpriteNode::SetPath(const std::string &a_sPath)
{
    //this->m_pCocosEntity = Sprite::create(a_sPath);
    //CCameraFeedNode::DisplayPicture, this, LmJniCppFacade::getCurrentPicturePath()
    ON_CC_THREAD(CSpriteNode::DisplayNewImage, this, a_sPath);
}

void CSpriteNode::DisplayNewImage(const std::string &a_sPath)
{
    m_sSpriteFilename = a_sPath;
    CSceneNode* pSceneNode = GetParentSceneNode();
    pSceneNode->UnInit();
    pSceneNode->Init();
}

void CSpriteNode::ChangeAnchor(int a_anchor)
{
    this->m_eAnchor = IntToAnchor(a_anchor);
    CSceneNode* pSceneNode = GetParentSceneNode();
    pSceneNode->UnInit();
    pSceneNode->Init();
}


} // namespace LM
