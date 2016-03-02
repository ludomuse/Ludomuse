#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"


using namespace cocos2d;


namespace LM
{

	CTouchBeganVisitor::CTouchBeganVisitor(Touch* a_pTouch, Event* a_pEvent, CKernel* a_pKernel) :
		CFindEntityTouchVisitor(a_pTouch, Desc<CEntityNode>(), "Touch"),
	    m_pEvent(a_pEvent),
		m_pKernel(a_pKernel)
{
}



bool CTouchBeganVisitor::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
	if (m_pEntityToFind.IsValid())
	{

		// must release the entity at the end of the sequence, after animations have ended
		CEntityNode* pEntity = m_pEntityToFind.Get();
		auto fpReleaseEntity = CallFunc::create([pEntity]() {
			CEntityNode::Release(pEntity);
		});

		if (m_sListenEvent == "Touch")
		{
			Vec2 oTouchLocation = a_pTouch->getLocation();
			Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				pEntity->Dispatch(m_sListenEvent);
			}

			TouchStop(pEntity);


		}
		else if (m_sListenEvent == "Move")
		{
			// Traverse the tree to find a drop area
			CEntityNode* pDropEntity = m_pKernel->FindEntity(a_pTouch, "Drop");
			if (pDropEntity)
			{
				pDropEntity->Dispatch("Drop");
				
				DropEntity(pEntity);
			}
			else
			{
				// if not intersecting a drop area :
				MoveEntityBack(pEntity);
			}

		}
	}
	return true;
}

bool CTouchBeganVisitor::OnTouchMove(Touch* a_pTouch, Event* a_pEvent)
{
	if (m_pEntityToFind.IsValid())
	{
		if (m_sListenEvent == "Touch")
		{
			// if listen to touch change the entity when leaving it
			Vec2 oTouchLocation = a_pTouch->getLocation();
			Rect oBoundingBox = m_pEntityToFind.Get()->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				TouchMoveIn(m_pEntityToFind.Get());
			}
			else
			{
				TouchMoveOut(m_pEntityToFind.Get());
			}
		}
		else if (m_sListenEvent == "Move")
		{
			MoveEntity(a_pTouch, m_pEntityToFind.Get());
		}
	}
	return true;
}





void CTouchBeganVisitor::MoveEntity(Touch* a_pTouch, CEntityNode* a_pEntity)
{
	Vec2 oMoveVector = a_pTouch->getLocation() - a_pTouch->getPreviousLocation();
	Vec2 oEntityLocation = a_pEntity->GetCocosEntity()->getPosition();
	a_pEntity->GetCocosEntity()->setPosition(oEntityLocation + oMoveVector);
	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			MoveEntity(a_pTouch, pEntity);
		}
	}
}



void CTouchBeganVisitor::TouchMoveOut(CEntityNode* a_pEntity)
{

	auto oTintTo = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);
	a_pEntity->GetCocosEntity()->runAction(oTintTo);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			TouchMoveOut(pEntity);
		}
	}
}

void CTouchBeganVisitor::TouchMoveIn(CEntityNode* a_pEntity)
{
	auto oTintTo = TintTo::create(0.0f, 120.0f, 120.0f, 120.0f);
	a_pEntity->GetCocosEntity()->runAction(oTintTo);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			TouchMoveIn(pEntity);
		}
	}
}


void CTouchBeganVisitor::TouchStop(CEntityNode* a_pEntity)
{

	auto fpReleaseEntity = CallFunc::create([a_pEntity]() {
		CEntityNode::Release(a_pEntity);
	});

	auto oTintTo = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);
	auto oSequence = Sequence::create(oTintTo, fpReleaseEntity, nullptr);

	a_pEntity->GetCocosEntity()->runAction(oSequence);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			TouchStop(pEntity);
		}
	}
}


void CTouchBeganVisitor::MoveEntityBack(CEntityNode* a_pEntity)
{

	auto fpReleaseEntity = CallFunc::create([a_pEntity]() {
		CEntityNode::Release(a_pEntity);
	});
	auto oMoveTo = MoveTo::create(0.25, a_pEntity->GetEntityStartLocation());
	auto oMoveToEase = EaseOut::create(oMoveTo->clone(), 0.5);

	auto oScaleTo = ScaleTo::create(0.25, a_pEntity->GetEntityStartScale());

	auto oSpawn = Spawn::createWithTwoActions(oMoveTo, oScaleTo);
	auto oSequence = Sequence::create(oSpawn, fpReleaseEntity, nullptr);

	a_pEntity->GetCocosEntity()->runAction(oSequence);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			MoveEntityBack(pEntity);
		}
	}
}



void CTouchBeganVisitor::DropEntity(CEntityNode* a_pEntity)
{

	auto fpReleaseEntity = CallFunc::create([a_pEntity]() {
		CEntityNode::Release(a_pEntity);
	});

	auto oFadeOut = FadeOut::create(1.0f);
	auto oSequence = Sequence::create(oFadeOut, fpReleaseEntity, nullptr);

	a_pEntity->GetCocosEntity()->runAction(oSequence);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			DropEntity(pEntity);
		}
	}
}


Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {

    // Check if the entity intersects the touch event
	  Vec2 oTouchLocation = m_pTouch->getStartLocation();
    Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
    if (oBoundingBox.containsPoint(oTouchLocation) && !pEntity->IsLocked() && pEntity->IsVisible())
    {
      // if so and if listenning to touch/move, store the entity
      if (pEntity->IsListeningTo("Touch"))
      {

		  m_pEntityToFind.Set(pEntity);
		  m_sListenEvent = "Touch";

		  StartTouch(m_pEntityToFind.Get());

		  return RESULT_PRUNE;
      }
	  else if (pEntity->IsListeningTo("Move"))
	  {

		  m_pEntityToFind.Set(pEntity);
		  m_sListenEvent = "Move";

		  StartMove(m_pEntityToFind.Get());

		  return RESULT_PRUNE;
	  }
      
    }
	else
	{
		return RESULT_PRUNE;
	}
  }
  
  return RESULT_CONTINUE;
}


void CTouchBeganVisitor::StartTouch(CEntityNode* a_pEntity)
{
	CEntityNode::Lock(a_pEntity);

	auto oTintTo = TintTo::create(0.0f, 120.0f, 120.0f, 120.0f);
	a_pEntity->GetCocosEntity()->runAction(oTintTo);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			StartTouch(pEntity);
		}
	}
}

void CTouchBeganVisitor::StartMove(CEntityNode* a_pEntity)
{
	CEntityNode::Lock(a_pEntity);

	auto oScaleTo1 = ScaleTo::create(0.1f, 2 * a_pEntity->GetEntityStartScale());
	auto oEaseOutBack1 = EaseBackOut::create(oScaleTo1);

	auto oScaleTo2 = ScaleTo::create(0.1f, 1.5 * a_pEntity->GetEntityStartScale());

	auto oSequence = Sequence::create(oEaseOutBack1, oScaleTo2, nullptr);

	a_pEntity->GetCocosEntity()->runAction(oSequence);

	for (CNode* itNode : *a_pEntity)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(itNode);
		if (pEntity)
		{
			StartMove(pEntity);
		}
	}
}




} // namespace LM
