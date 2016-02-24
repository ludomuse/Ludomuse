#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"


using namespace cocos2d;


namespace LM
{

	CTouchBeganVisitor::CTouchBeganVisitor(Touch* a_pTouch, Event* a_pEvent, CKernel* a_pKernel) :
    m_pTouch(a_pTouch),
    m_pEvent(a_pEvent),
	m_bStopVisiting(false),
	m_pTouchBeganEntity(nullptr),
	m_pKernel(a_pKernel)
{
}


void CTouchBeganVisitor::Traverse(CNode* a_pNode)
{
  if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
  {
    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
    CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
    if (pEntity || pScene)
    {
      // traverse in decreasing order of z-index
		std::vector<CNode*> oNodes = a_pNode->GetChildren();
		for (int i = oNodes.size() - 1; i >= 0; --i)
		{
			// stop traversing the tree
			if (m_bStopVisiting) return;
			
			Traverse(oNodes[i]);
		}
    }
    else
    {
      Traverse(a_pNode->GetCurrentNode());
    }

  }

  ProcessNodeBottomUp(a_pNode);

}


bool CTouchBeganVisitor::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
	if (m_pTouchBeganEntity)
	{

		// must release the entity at the end of the sequence, after animations have ended
		CEntityNode* pEntity = m_pTouchBeganEntity;
		auto fpReleaseEntity = CallFunc::create([pEntity]() {
			CEntityNode::Release(pEntity);
		});

		if (m_sListenEvent == "Touch")
		{
			Vec2 oTouchLocation = a_pTouch->getLocation();
			Rect oBoundingBox = m_pTouchBeganEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				m_pTouchBeganEntity->Dispatch(m_sListenEvent);
			}

			auto oTintTo = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);

			auto oSequence = Sequence::create(oTintTo, fpReleaseEntity, nullptr);

			m_pTouchBeganEntity->GetCocosEntity()->runAction(oSequence);


		}
		else if (m_sListenEvent == "Move")
		{
			// Traverse the tree to find a drop area
			CEntityNode* pDropEntity = m_pKernel->FindEntity(a_pTouch, "Drop");
			if (pDropEntity)
			{
				pDropEntity->Dispatch("Drop");

				auto oFadeOut = FadeOut::create(1.0f);
				auto oSequence = Sequence::create(oFadeOut, fpReleaseEntity, nullptr);

				pEntity->GetCocosEntity()->runAction(oSequence);
			}
			else
			{
				// if not intersecting a drop area :
				auto oMoveTo = MoveTo::create(0.25, m_oEntityPosition);
				auto oMoveToEase = EaseOut::create(oMoveTo->clone(), 0.5);

				auto oScaleTo = ScaleTo::create(0.25, m_fEntityScale);

				auto oSpawn = Spawn::createWithTwoActions(oMoveTo, oScaleTo);
				auto oSequence = Sequence::create(oSpawn, fpReleaseEntity, nullptr);

				m_pTouchBeganEntity->GetCocosEntity()->runAction(oSequence);

			}

		}
	}
	return true;
}

bool CTouchBeganVisitor::OnTouchMove(Touch* a_pTouch, Event* a_pEvent)
{
	if (m_pTouchBeganEntity)
	{
		if (m_sListenEvent == "Touch")
		{
			// if listen to touch change the entity when leaving it
			Vec2 oTouchLocation = m_pTouch->getLocation();
			Rect oBoundingBox = m_pTouchBeganEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				auto oTintTo = TintTo::create(0.0f, 120.0f, 120.0f, 120.0f);
				m_pTouchBeganEntity->GetCocosEntity()->runAction(oTintTo);
			}
			else
			{
				auto oTintTo = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);
				m_pTouchBeganEntity->GetCocosEntity()->runAction(oTintTo);
			}

		}
		else if (m_sListenEvent == "Move")
		{
			Vec2 oMoveVector = a_pTouch->getLocation() - a_pTouch->getPreviousLocation();
			Vec2 oEntityLocation = m_pTouchBeganEntity->GetCocosEntity()->getPosition();
			m_pTouchBeganEntity->GetCocosEntity()->setPosition(oEntityLocation + oMoveVector);
		}
	}
	return true;
}


Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {

    // Check if the entity intersects the touch event
	  Vec2 oTouchLocation = m_pTouch->getStartLocation();
    Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
    if (oBoundingBox.containsPoint(oTouchLocation) && !pEntity->IsLocked())
    {
      // if so and if listenning to touch/move, store the entity
      if (pEntity->IsListeningTo("Touch"))
      {

		  CEntityNode::Lock(pEntity);

		  m_pTouchBeganEntity = pEntity;
		  m_sListenEvent = "Touch";

		  auto oTintTo = TintTo::create(0.0f, 120.0f, 120.0f, 120.0f);
		  m_pTouchBeganEntity->GetCocosEntity()->runAction(oTintTo);

		  return RESULT_PRUNE;
      }
	  else if (pEntity->IsListeningTo("Move"))
	  {

		  CEntityNode::Lock(pEntity);

		  m_pTouchBeganEntity = pEntity;
		  m_sListenEvent = "Move";
		  m_oEntityPosition = m_pTouchBeganEntity->GetCocosEntity()->getPosition();
		  m_fEntityScale = m_pTouchBeganEntity->GetCocosEntity()->getScale();

		  auto oScaleTo1 = ScaleTo::create(0.1f, 2 * m_fEntityScale);
		  auto oEaseOutBack1 = EaseBackOut::create(oScaleTo1);

		  auto oScaleTo2 = ScaleTo::create(0.1f, 1.5 * m_fEntityScale);

		  auto oSequence = Sequence::create(oEaseOutBack1, oScaleTo2, nullptr);

		  m_pTouchBeganEntity->GetCocosEntity()->runAction(oSequence);
		  return RESULT_PRUNE;
	  }
      
    }
  }
  
  return RESULT_CONTINUE;
}

Result CTouchBeganVisitor::ProcessNodeBottomUp(CNode* a_pNode)
{
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
	if (pEntity)
	{
		Vec2 oTouchLocation = m_pTouch->getLocation();
		Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
		if (oBoundingBox.containsPoint(oTouchLocation))
		{
			m_bStopVisiting = true;
			return RESULT_STOP;
		}
	}
}


} // namespace LM
