#include <assert.h>
#include "../Include/CSpriteNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CMacroManager.h"
#ifdef LUDOMUSE_EDITOR
#include <QDebug>
#include <CProjectManager.h>

#include <vector>
#endif
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
  Sprite* pSprite = Sprite::create(CMacroManager::Instance()->CheckDefinition(m_sSpriteFilename));
  //m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));

	  if (!m_bColored)
	  {
		  BlendFunc oColoredBlend;
		  oColoredBlend.src = GL_ZERO;
		  oColoredBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
		  pSprite->setBlendFunc(oColoredBlend);
	  }
	  //else
	  //{
		 // BlendFunc oColoredBlend;
		 // oColoredBlend.src = GL_SRC_ALPHA;
		 // oColoredBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
		 // pBlendedNode->setBlendFunc(oColoredBlend);
	  //}
    CCLOG("chargement de l'image : %s", m_sSpriteFilename.c_str());
    assert(pSprite && "Erreur : l'image n'a pas pu etre trouvée dans le dossier de scenario");
	m_pCocosEntity = pSprite;
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
    m_sSpriteFilename = a_sPath;
    ON_CC_THREAD(CSpriteNode::Update, this);
}

//void CSpriteNode::DisplayNewImage()
//{
//    CSceneNode* pSceneNode = GetParentSceneNode();
//    pSceneNode->UnInit();
//    pSceneNode->Init();
//}

//void CSpriteNode::ChangeAnchor(int a_anchor)
//{
//    this->m_eAnchor = IntToAnchor(a_anchor);
//    CSceneNode* pSceneNode = GetParentSceneNode();
//    pSceneNode->UnInit();
//    pSceneNode->Init();
//}

//int CSpriteNode::GetAnchor()
//{
//    return this->m_eAnchor;
//}
#ifdef LUDOMUSE_EDITOR
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
void CSpriteNode::Copy(CEntityNode* a_pSprite, bool a_bRecCopy)
{
    CSpriteNode* pSprite = dynamic_cast<CSpriteNode*>(a_pSprite);
    if (pSprite)
    {
        m_sSpriteFilename = pSprite->GetPath();
    }
    CEntityNode::Copy(a_pSprite, a_bRecCopy);
}

bool CSpriteNode::UseFile(const std::string& a_sFilename)
{
    return m_sSpriteFilename == a_sFilename || CEntityNode::UseFile(a_sFilename);
}

} // namespace LM
