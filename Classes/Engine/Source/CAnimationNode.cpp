#include "../Include/CAnimationNode.h"
#include "../../Modules/Util/Include/Util.h"


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

    std::string file = StringSplit(LM::basename(m_sSpriteSheet.c_str()), '.')[0];

    Vector<SpriteFrame*> animFrames(50);
    char str[100] = {0};
    for (int i = 0; i < 50; ++i)
    {
        sprintf(str, "%s_%05d.png", file.c_str(), i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    m_pAnimation = Animation::createWithSpriteFrames(animFrames, 0.04f);

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
    //m_pSpriteBatchNode->addChild(sprite);

    m_pCocosEntity = sprite;
    PopulateParent();


    sprite->runAction(RepeatForever::create(Animate::create(m_pAnimation)));


    CNode::Init();
}



} // namespace LM
