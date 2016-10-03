#include "../Include/CSceneNode.h"
#include "../Include/CSpriteNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CMenuNode.h"

#include "../Include/CKernel.h"

using namespace cocos2d;

namespace LM
{

CSceneNode::CSceneNode(std::string a_sID, CKernel* a_pKernel) : 
	m_sID(a_sID),
	m_bIsSynced(false), 
	m_bDashboardTrigger(false),
	m_pKernel(a_pKernel)
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

bool CSceneNode::init()
{
  if (!cocos2d::Layer::init())
  {
    return false;
  }

  

  CCLOG("init scene : %s", m_sID.c_str());
  
  CNode::Init();

  if (m_pKernel && m_pKernel->m_bDebugMode && m_sID != "none")
	  DisplayDebugInfo();


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
	Size oVisibleSize = Director::getInstance()->getVisibleSize();

	pLabel->setAlignment(TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0, 1));
	pLabel->setPosition(Vec2(20 + oOrigin.x, oOrigin.y + oVisibleSize.height - 20));

	m_pScene->addChild(pLabel, 2);



	// TODO : add "quick travel" box
	m_pQuickBox = ui::EditBox::create(Size(oVisibleSize.width / 2.0f,
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


void CSceneNode::ToggleQuickBox()
{
	m_pQuickBox->setVisible(!m_pQuickBox->isVisible());
	m_pQuickButton->setVisible(!m_pQuickButton->isVisible());
}

} // namespace LM
