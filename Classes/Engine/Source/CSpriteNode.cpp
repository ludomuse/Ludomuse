#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CLabelNode.h"

#include <QDebug>

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

int CSpriteNode::GetAnchor()
{
    return this->m_eAnchor;
}

void CSpriteNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    // TODO
    rapidjson::Value spriteNode(rapidjson::kObjectType);
    spriteNode.AddMember("type", "Image", a_rAllocator);
    if(!m_sID.empty())
    {
        spriteNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("source", rapidjson::Value(m_sSpriteFilename.c_str(), m_sSpriteFilename.length()) , a_rAllocator);
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

} // namespace LM
