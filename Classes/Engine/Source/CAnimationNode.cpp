#include "../Include/CAnimationNode.h"
#include "../../Modules/Util/Include/Util.h"
#include "CProjectManager.h"


using namespace cocos2d;


namespace LM
{


CAnimationNode::CAnimationNode(const std::string& a_rSpriteSheet,
                               const std::string& a_rFrameFile,
                               const EAnchor a_eAnchor,
                               int a_iWidth,
                               int a_iHeight,
                               int a_iXPosition,
                               int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_sSpriteSheet(a_rSpriteSheet),
    m_sFrameFile(a_rFrameFile)
{
    m_pSpriteBatchNode = SpriteBatchNode::create(m_sSpriteSheet);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_sFrameFile);

    std::string file = StringSplit(basename(m_sSpriteSheet.c_str()), '.')[0];

    Vector<SpriteFrame*> animFrames(51);
    char str[100] = {0};
    for (int i = 0; i < 51; ++i)
    {
        sprintf(str, "%s_%05d.png", file.c_str(), i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    m_pAnimation = Animation::createWithSpriteFrames(animFrames, 0.0255f);

    m_pSpriteBatchNode->retain();
    m_pAnimation->retain();
}


CAnimationNode::~CAnimationNode()
{
    m_pSpriteBatchNode->release();
    m_pAnimation->release();
}



void CAnimationNode::Init()
{

    //AnimationCache::getInstance()->addAnimationsWithFile(m_sAnimationFile);

    std::string file = StringSplit(basename(m_sSpriteSheet.c_str()), '.')[0];

    Sprite* sprite = Sprite::createWithSpriteFrameName(file + "_00000.png");
    m_pSpriteBatchNode->addChild(sprite);

    m_pCocosEntity = sprite;
    PopulateParent();


    sprite->runAction(RepeatForever::create(Animate::create(m_pAnimation)));


    CNode::Init();
}


void CAnimationNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{
    rapidjson::Value animation(rapidjson::kObjectType);
    animation.AddMember("type", "Animation", a_rAllocator);

    if (!m_sID.empty())
    {
        animation.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);

    std::string projectPath = CProjectManager::Instance()->GetProjectPath();

    std::string temp = m_sFrameFile;
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
    params.AddMember("animation", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);

    temp = m_sSpriteSheet;
    index = temp.find(projectPath);
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
    string = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("sheet", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);


    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

    if (!m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if (!m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    animation.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(animation, a_rAllocator);
}



} // namespace LM
