#include "../Include/CSceneNode.h"
#include "../Include/CSpriteNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CMenuNode.h"
#include "../Include/CInfoNode.h"
#include "../Include/CValidator.h"
#include "../Include/CKernel.h"
#ifdef LUDOMUSE_EDITOR
#include <functional>

#include <QDebug>
#include <CProjectManager.h>
#endif

using namespace cocos2d;

namespace LM
{

CSceneNode::CSceneNode(const std::string& a_rID, CKernel* a_pKernel) :
    m_sID(a_rID),
    m_bIsSynced(false),
    m_bDashboardTrigger(false),
    m_pKernel(a_pKernel),
    m_sRewardID(""),
    m_sInitSound(""),
    m_sValidSound("")
{
}

Scene* CSceneNode::CreateScene()
{
    // autoreleased
    m_pScene = Scene::create();
    // autoreleased
    auto oLayer = CSceneNode::create();

    m_pScene->addChild(oLayer);

    return m_pScene;

}


cocos2d::Scene* CSceneNode::GetScene()
{
    return m_pScene;
}

std::string CSceneNode::GetID()
{
    return m_sID;
}

bool CSceneNode::init()
{
    if (!cocos2d::Layer::init())
    {
        return false;
    }



    CCLOG("init scene : %s", m_sID.c_str());

    CNode::Init();

#ifndef LUDOMUSE_EDITOR
    if (m_pKernel && m_pKernel->m_bDebugMode && m_sID != "none")
        DisplayDebugInfo();
#endif

    return true;

}



const std::string& CSceneNode::GetSceneID() const
{
    return m_sID;
}

void CSceneNode::SetSynced(bool a_bIsSynced)
{
    m_bIsSynced = a_bIsSynced;
}

bool CSceneNode::IsSynced()
{
    return m_bIsSynced;
}



void CSceneNode::DisplayDebugInfo()
{
    Label* pLabel = Label::createWithTTF(m_sID, "fonts/arial.ttf", 12);

    Vec2 oOrigin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size oVisibleSize = Director::getInstance()->getVisibleSize();

    pLabel->setAlignment(TextHAlignment::LEFT);
    pLabel->setAnchorPoint(Vec2(0, 1));
    pLabel->setPosition(Vec2(20 + oOrigin.x, oOrigin.y + oVisibleSize.height - 20));

    m_pScene->addChild(pLabel, 2);



	// TODO : add "quick travel" box
    m_pQuickBox = ui::EditBox::create(cocos2d::Size(oVisibleSize.width / 2.0f,
		oVisibleSize.height / 5.0f),
		ui::Scale9Sprite::create("ui/textfieldBackground.png"));
	m_pQuickBox->setFont("fonts/arial.ttf", 40);
	m_pQuickBox->setFontColor(Color3B::BLACK);
	m_pQuickBox->setMaxLength(50);
	m_pQuickBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pQuickBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

    m_pQuickBox->setAnchorPoint(Vec2(0, 1));
    m_pQuickBox->setPosition(Vec2(oOrigin.x, oOrigin.y + oVisibleSize.height/2.0f));

    m_pScene->addChild(m_pQuickBox, 2);


    m_pQuickButton = MenuItemImage::create("ui/ok.png", "ui/ok.png",
                                           [this](Ref* pSender) -> void
    {
            CCLOG("goto scene button clicked : %s", m_pQuickBox->getText());
            if (m_pKernel)
    {
            m_pQuickButton->setVisible(false);
            m_pQuickBox->setVisible(false);

            SEvent oGotoSceneEvent;
            oGotoSceneEvent.m_sStringValue = m_pQuickBox->getText();
            std::transform(oGotoSceneEvent.m_sStringValue.begin(),
                           oGotoSceneEvent.m_sStringValue.end(),
                           oGotoSceneEvent.m_sStringValue.begin(),
                           ::tolower);
            m_pKernel->GotoScreenID(oGotoSceneEvent, nullptr);
}
});

    m_pQuickButton->setPosition(Vec2::ZERO);
    m_pQuickButton->setScale(2.0f);

    Menu* menu = Menu::create(m_pQuickButton, NULL);
    m_pScene->addChild(menu, 3);


    m_pQuickBox->setVisible(false);
    m_pQuickButton->setVisible(false);

}


std::string CSceneNode::GetSceneID(){
   return this->m_sID;
}

void CSceneNode::ToggleQuickBox()
{
    m_pQuickBox->setVisible(!m_pQuickBox->isVisible());
    m_pQuickButton->setVisible(!m_pQuickButton->isVisible());
}
#ifdef LUDOMUSE_EDITOR
void CSceneNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    rapidjson::Value newScene(rapidjson::kObjectType);
    newScene.AddMember("scene", rapidjson::Value(this->m_sID.c_str(), this->m_sID.length()), a_rAllocator);
    if(this->m_bDashboardTrigger)
    {
        newScene.AddMember("dashboardTrigger", rapidjson::Value(true), a_rAllocator);
    }
    if(this->m_bIsSynced)
    {
        newScene.AddMember("sync", rapidjson::Value(true), a_rAllocator);
    }
    if(this->m_sRewardID != "")
    {
        newScene.AddMember("rewardID", rapidjson::Value(this->m_sRewardID.c_str(), this->m_sRewardID.length()), a_rAllocator);
    }
    if(this->m_sInitSound != "")
    {
        std::string* sInitSound = new std::string(CProjectManager::Instance()->GetRelativePathForFile(m_sInitSound));
        newScene.AddMember("initSound", rapidjson::Value(sInitSound->c_str(), sInitSound->length()), a_rAllocator);
    }
    if(this->m_sValidSound != "")
    {
        std::string* sValidSound = new std::string(CProjectManager::Instance()->GetRelativePathForFile(m_sValidSound));
        newScene.AddMember("validSound", rapidjson::Value(sValidSound->c_str(), sValidSound->length()), a_rAllocator);
    }
    rapidjson::Value navigation(rapidjson::kArrayType);
    rapidjson::Value content(rapidjson::kArrayType);
    rapidjson::Value information(rapidjson::kArrayType);


    for(CNode* currentNode : this->m_vChildren)
    {
        // Special process (navigation and information)
        //        qDebug("cast en CMenuNode");
        CMenuNode* pMenuNode = dynamic_cast<CMenuNode*>(currentNode);
        if(pMenuNode)
        {
            pMenuNode->ToJson(navigation, a_rAllocator);
            continue;
        }
        CValidator* pValidator = dynamic_cast<CValidator*>(currentNode);
        if(pValidator)
        {
            pValidator->ToJson(navigation, a_rAllocator);
            continue;
        }

        //        qDebug("cast en CInfoNode");
        CInfoNode* pInfoNode = dynamic_cast<CInfoNode*>(currentNode);
        if(pInfoNode)
        {
            pInfoNode->ToJson(information, a_rAllocator);
            continue;
        }
        //        qDebug("Default comp");
        // Else do default process (content)
        currentNode->ToJson(content, a_rAllocator);

    }

    newScene.AddMember("navigation", navigation, a_rAllocator);
    newScene.AddMember("content", content, a_rAllocator);
    newScene.AddMember("information", information, a_rAllocator);


    a_rParent.PushBack(newScene, a_rAllocator);
}
#endif
bool CSceneNode::hasID(const std::string &a_rID)
{
    if(this->m_sID == a_rID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSceneNode::SaveImage(std::function<void(RenderTexture*, const std::string&)> callback,
                           float a_fScale)
{
#ifdef LUDOMUSE_EDITOR
    RenderTexture* renderTexture = RenderTexture::create(m_pScene->getContentSize().width*a_fScale,
                                                         m_pScene->getContentSize().height*a_fScale,
                                                         Texture2D::PixelFormat::RGBA8888);
    renderTexture->begin();
    m_pScene->setScale(a_fScale);
    m_pScene->visit();
    renderTexture->end();

    renderTexture->saveToFile(CProjectManager::Instance()->GetRelativeWritablePath() + "/" + m_sID + ".png", true, callback);
    Director::getInstance()->getRenderer()->render();
#endif
}

std::string CSceneNode::GetRewardID()
{
    return m_sRewardID;
}

void CSceneNode::SetRewardID(std::string a_sRewardID)
{
    m_sRewardID = a_sRewardID;
}

std::string CSceneNode::GetInitSound()
{
    return m_sInitSound;
}

void CSceneNode::SetInitSound(std::string a_sInitSound)
{
    m_sInitSound = a_sInitSound;
}

std::string CSceneNode::GetValidSound()
{
    return m_sValidSound;
}

void CSceneNode::SetValidSound(std::string a_sValidSound)
{
    m_sValidSound = a_sValidSound;
}
#ifdef LUDOMUSE_EDITOR
bool CSceneNode::UseFile(const std::string &a_sFilename)
{
    return a_sFilename == m_sInitSound || a_sFilename == m_sValidSound;
}
#endif
} // namespace LM


