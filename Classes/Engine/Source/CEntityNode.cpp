#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"

#include <QDebug>

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CEntityNode::CEntityNode(EAnchor a_eAnchor, int a_iWidth, int a_iHeight,
						 int a_iXPosition, int a_iYPosition, const std::string& a_sID) :
    m_eAnchor(a_eAnchor),
	m_iXPosition(a_iXPosition),
    m_iYPosition(a_iYPosition),
	m_iWidth(a_iWidth),
	m_iHeight(a_iHeight),
	m_bVisible(true),
	m_bLocked(false),
	m_sID(a_sID),
	m_fEntityStartScale(0),
	m_pCocosEntity(nullptr)
{
}



void CEntityNode::UnInit(bool removeChild)
{
	//m_pCocosEntity->autorelease();
	//m_pCocosEntity->release();
	if (m_pCocosEntity != nullptr) {
		if(removeChild)
			GetParentScene()->removeChild(m_pCocosEntity);

		m_pCocosEntity = nullptr;
		CNode::UnInit(removeChild);
	}
	else {
		return;
	}
}



void CEntityNode::Revert(bool a_bVisible)
{
	Show(a_bVisible);
	if (m_sID != "")
		CCLOG("reverting : %s", m_sID.c_str());
	m_pCocosEntity->setPosition(m_oEntityStartLocation);
	m_pCocosEntity->setScale(m_fEntityStartScale);
        for (CNode* itNode : m_vChildren)
        {
          CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
          if (pEntity)
          {
            pEntity->Revert(a_bVisible);
          }
        }
}


cocos2d::Node* CEntityNode::GetCocosEntity()
{
	return m_pCocosEntity;
}


void CEntityNode::AddListener(const std::string& a_rEvent, const CEventCallback& a_rCallback)
{
	std::map<std::string, std::vector<CEventCallback>>::iterator it = m_mListeners.find(a_rEvent);
	if (it != m_mListeners.end())
	{
		it->second.push_back(a_rCallback);
	}
	else
	{
		std::vector<CEventCallback> oCallbacks;
		oCallbacks.push_back(a_rCallback);
		m_mListeners.insert(std::pair<std::string, std::vector<CEventCallback>>(a_rEvent, oCallbacks));
	}
}

void CEntityNode::DisableEvent(const std::string& a_rEvent)
{
	m_oDisabledEvents.insert(a_rEvent);
}

void CEntityNode::EnableEvent(const std::string& a_rEvent)
{
	m_oDisabledEvents.erase(a_rEvent);
}


bool CEntityNode::EventIsDisabled(const std::string& a_rEvent)
{
	return (m_oDisabledEvents.find(a_rEvent) != m_oDisabledEvents.end());
}

bool CEntityNode::IsListeningTo(const std::string& a_rEvent)
{
	if (m_oDisabledEvents.find(a_rEvent) == m_oDisabledEvents.end())
	{
		std::map<std::string, std::vector<CEventCallback>>::iterator it = m_mListeners.find(a_rEvent);
		return (it != m_mListeners.end());
	}
}


void CEntityNode::Dispatch(const std::string& a_rEvent, CEntityNode* a_pSender)
{
	if (m_oDisabledEvents.find(a_rEvent) == m_oDisabledEvents.end())
	{
		std::map<std::string, std::vector<CEventCallback>>::iterator it = m_mListeners.find(a_rEvent);
		if (it != m_mListeners.end())
		{
			for (CEventCallback oCallback : it->second)
			{
				CCLOG("CEntity::Dispatch : Calling callback %s on entity %s", a_rEvent.c_str(), m_sID.c_str());
				oCallback(a_pSender);
			}
		}
	}
}

void CEntityNode::PopulateParent(bool a_bDoScaling)
{

	// Size oVisibleSize = Director::getInstance()->getVisibleSize();
	// Vec2 oOrigin = Director::getInstance()->getVisibleOrigin();
  Size oVisibleSize = GetParentVisibleSize();
  Vec2 oOrigin = GetParentOrigin();
	
	switch (m_eAnchor)
	{
	case LM::CENTER:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width/2, oOrigin.y + oVisibleSize.height/2));
		break;
	case LM::BOTTOM_LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y));
		break;
	case LM::LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y + oVisibleSize.height / 2));
		break;
	case LM::TOP_LEFT:
		m_pCocosEntity->setAnchorPoint(Vec2(0, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x, oOrigin.y + oVisibleSize.height));
		break;
	case LM::TOP:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width / 2, oOrigin.y + oVisibleSize.height));
		break;
	case LM::TOP_RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 1));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y + oVisibleSize.height));
		break;
	case LM::RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 0.5));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y + oVisibleSize.height / 2));
		break;
	case LM::BOTTOM_RIGHT:
		m_pCocosEntity->setAnchorPoint(Vec2(1, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width, oOrigin.y));
		break;
	case LM::BOTTOM:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0));
		m_pCocosEntity->setPosition(Vec2(oOrigin.x + oVisibleSize.width / 2, oOrigin.y));
		break;
	case LM::FLOAT:
		m_pCocosEntity->setAnchorPoint(Vec2(0.5, 0.5));
		m_pCocosEntity->setPosition(Vec2(m_iXPosition, m_iYPosition));
		break;
	default:
		break;
	}


	if (a_bDoScaling) {

		float fNewScale;

		if (m_iWidth != 0)
		{
			float fOldWidth = m_pCocosEntity->getBoundingBox().getMaxX() - m_pCocosEntity->getBoundingBox().getMinX();
			float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
			fNewScale = fNewWidth / fOldWidth;
		}
		else 
		{
			float fOldHeight = m_pCocosEntity->getBoundingBox().getMaxY() - m_pCocosEntity->getBoundingBox().getMinY();
			float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);
			fNewScale = fNewHeight / fOldHeight;
		}



		m_pCocosEntity->setScale(fNewScale);

	}
	cocos2d::Scene* pScene = GetParentScene();
	if (pScene)
	{
		pScene->addChild(m_pCocosEntity, 0);
	}


	m_pCocosEntity->setVisible(m_bVisible);

	m_oEntityStartLocation = m_pCocosEntity->getPosition();
	m_fEntityStartScale = m_pCocosEntity->getScale();

	Dispatch("Init");
}


Scene* CEntityNode::GetParentScene()
{
    return GetParentSceneNode()->GetScene();
}


CSceneNode* CEntityNode::GetParentSceneNode()
{
    CSceneNode* pParentScene = dynamic_cast<CSceneNode*>(m_pParent);
    if (pParentScene)
    {
        return pParentScene;
    }
    else
    {
        CEntityNode* pParentNode = dynamic_cast<CEntityNode*>(m_pParent);
        if (pParentNode)
        {
            return pParentNode->GetParentSceneNode();
        }
    }
}

Size CEntityNode::GetParentVisibleSize()
{

  CEntityNode* pParentEntity = dynamic_cast<CEntityNode*>(m_pParent);
  if (pParentEntity)
  {
    return pParentEntity->GetVisibleSize();
  }
  
  return Director::getInstance()->getVisibleSize();
  
}

Vec2 CEntityNode::GetParentOrigin()
{
  CEntityNode* pParentEntity = dynamic_cast<CEntityNode*>(m_pParent);
  if (pParentEntity)
  {
    return pParentEntity->GetOrigin();
  }

  return Director::getInstance()->getVisibleOrigin();
}


Size CEntityNode::GetVisibleSize()
{
	//return m_pCocosEntity->getContentSize();
	Rect oBoundingBox = m_pCocosEntity->getBoundingBox();
	return Size(oBoundingBox.getMaxX() - oBoundingBox.getMinX(),
		oBoundingBox.getMaxY() - oBoundingBox.getMinY());
}

Vec2 CEntityNode::GetOrigin()
{
	Rect oBoundingBox = m_pCocosEntity->getBoundingBox();
	return Vec2(oBoundingBox.getMinX(), oBoundingBox.getMinY());
}

void CEntityNode::SetVisible(bool a_bVisible)
{
	m_bVisible = a_bVisible;
}

bool CEntityNode::IsVisible()
{
	return m_bVisible;
}

std::string CEntityNode::GetID()
{
	return m_sID;
}

void CEntityNode::SetID(const std::string& a_rID)
{
	m_sID = a_rID;
}

void CEntityNode::Show(bool a_bVisible)
{
	if (GetID() != "" && a_bVisible)
		CCLOG("Showing entity : %s", GetID().c_str());

	m_bVisible = a_bVisible;
	if (m_pCocosEntity) 
	{
		m_pCocosEntity->setVisible(a_bVisible);
		FadeIn();
	}
	
	for (CNode* itNode : *this)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity && (!pEntity->IsListeningTo("Show") || !a_bVisible))
		{
			pEntity->Show(a_bVisible);
		}
	}

}

bool CEntityNode::IsLocked()
{
	return m_bLocked;
}

Vec2 CEntityNode::GetEntityStartLocation()
{
	return m_oEntityStartLocation;
}

float CEntityNode::GetEntityStartScale()
{
	return m_fEntityStartScale;
}


void CEntityNode::Fade()
{
	auto fpReleaseEntity = CallFunc::create([this]() {
		CEntityNode::Release(this);
		this->Revert();
	});

	auto oFadeOut = FadeOut::create(1.0f);
	auto oSequence = Sequence::create(oFadeOut, fpReleaseEntity, nullptr);

	m_pCocosEntity->runAction(oSequence);

	for (CNode* itNode : m_vChildren)
	{
		CEntityNode* pChildEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pChildEntity)
		{
			pChildEntity->Fade();
		}
	}
}

void CEntityNode::FadeIn()
{
	CCLOG("CEntityNode::FadeIn %s", m_sID.c_str());
	auto oFadeIn = FadeIn::create(0.5f);

	m_pCocosEntity->runAction(oFadeIn);

	/*for (CNode* itNode : m_vChildren)
	{
		CEntityNode* pChildEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pChildEntity)
		{
			pChildEntity->FadeIn();
		}
	}*/
}


void CEntityNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    // NOTHING TO DO
}

void CEntityNode::ToJsonListener(rapidjson::Value& a_rListeners, rapidjson::Document::AllocatorType& a_rAllocator)
{
    std::map<std::string, std::vector<CEventCallback>>::iterator it;
    for(it = m_mListeners.begin();
        it != m_mListeners.end();
        ++it)
    {
        for(CEventCallback& currentEvent : it->second)
        {
            rapidjson::Value listener(rapidjson::kObjectType);
            const std::string &type = it->first;
            listener.AddMember("type", rapidjson::Value(type.c_str(), type.length()), a_rAllocator);
            rapidjson::Value params(rapidjson::kObjectType);
            params.AddMember("callback",
                             rapidjson::Value(currentEvent.getCallbackName().c_str(), currentEvent.getCallbackName().length()),
                             a_rAllocator);
            const SEvent& tamponEvent = currentEvent.getArg();
            switch(tamponEvent.m_eArg){
            case SEvent::NONE:
                break;
            case SEvent::STRING:
                params.AddMember("arg",
                                 rapidjson::Value(tamponEvent.m_sStringValue.c_str(), tamponEvent.m_sStringValue.length()),
                                 a_rAllocator);
                break;
            case SEvent::BOOLEAN:
                params.AddMember("arg", tamponEvent.m_bBoolValue,a_rAllocator);
                break;
            case SEvent::NUMBER:
                params.AddMember("arg", tamponEvent.m_iIntValue,a_rAllocator);
                break;
            }
            listener.AddMember("params", params, a_rAllocator);
            a_rListeners.PushBack(listener, a_rAllocator);
        }
    }


//	if (it != m_mListeners.end())
//	{
//		it->second.push_back(a_rCallback);
//	}
//	else
//	{
//		std::vector<CEventCallback> oCallbacks;
//		oCallbacks.push_back(a_rCallback);
//		m_mListeners.insert(std::pair<std::string, std::vector<CEventCallback>>(a_rEvent, oCallbacks));
//	}
}

bool CEntityNode::Lock(CEntityNode* a_pEntity)
{
	if (!a_pEntity->IsLocked())
	{
		a_pEntity->m_bLocked = true;
		for (CNode* itNode : *a_pEntity)
		{
			CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
			if (pEntity)
			{
				CEntityNode::Lock(pEntity);
			}
		}
		return true;
	}
	return false;
}

void CEntityNode::Release(CEntityNode* a_pEntity)
{
	a_pEntity->m_bLocked = false;
	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			CEntityNode::Release(pEntity);
		}
	}
}


} // namespace LM
