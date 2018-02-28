#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"
#include "../Include/CScratchNode.h"

#include "../../Modules/Util/Include/CStats.h"

using namespace cocos2d;


namespace LM
{

	CTouchBeganVisitor::CTouchBeganVisitor(Touch* a_pTouch, Event* a_pEvent, CKernel* a_pKernel) :
		CFindEntityTouchVisitor(a_pTouch, Desc<CNode>(), "Touch"),
		m_pEvent(a_pEvent),
		m_pKernel(a_pKernel),
		m_iTouchID(a_pTouch->getID())
{
	CCLOG("CTouchBeganVisitor::CTouchBeganVisitor touch id : %d", m_iTouchID);
}



bool CTouchBeganVisitor::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
	CCLOG("CTouchBeganVisitor::OnTouchEnd touch id : %d", a_pTouch->getID());
	if (a_pTouch->getID() != m_iTouchID)
		return false;

	if (m_pEntityToFind.IsValid())
	{

		// must release the entity at the end of the sequence, after animations have ended
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(m_pEntityToFind.Get());
		if (!pEntity)
			return false;
#ifdef LUDOMUSE_EDITOR
		auto fpReleaseEntity = CallFunc::create([pEntity]() {
			CEntityNode::Release(pEntity);
		});
#endif
		if (m_sListenEvent == "Touch")
		{
			Vec2 oTouchLocation = a_pTouch->getLocation();
			if (pEntity->GetCocosEntity() != nullptr)
			{
				Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				pEntity->Dispatch(m_sListenEvent);
				M_STATS_SCREEN.nbValidTouches++;
			}
			}
			TouchStop(pEntity);


		}
		else if (m_sListenEvent == "Move")
		{
			// Traverse the tree to find a drop area
			CEntityNode* pDropEntity = m_pKernel->FindEntity(a_pTouch, "Drop");
			if (pDropEntity)
			{

				M_STATS_SCREEN.nbValidDrops++;
				pDropEntity->Dispatch("Drop", pEntity);
				if (pEntity->IsListeningTo("Droped"))
				{
					// entity must be released after the Droped event 
					pEntity->Dispatch("Droped", pDropEntity);
				}
				else
				{
					CEntityNode::Release(pEntity);
				}
				pEntity->GetCocosEntity()->setGlobalZOrder(0);
			}
			//else if (pDropEntity = m_pKernel->FindEntity(a_pTouch, "Anchor"))
			//{
			//	AnchorEntity(pEntity, pDropEntity);
			//}
			else
			{
				// if not intersecting a drop area :
				M_STATS_SCREEN.nbInvalidDrops++;
				MoveEntityBack(pEntity);
			}

		}
		else if (m_sListenEvent == "CountdownPressed")
		{
			pEntity->Dispatch("CountdownReleased");
			TouchStop(pEntity);
		}
		else
		  {
		    CEntityNode::Release(pEntity);
		  }
	}
	return true;
}

bool CTouchBeganVisitor::OnTouchMove(Touch* a_pTouch, Event* a_pEvent)
{
	//CCLOG("CTouchBeganVisitor::OnTouchMove touch id : %d", a_pTouch->getID());
	if (a_pTouch->getID() != m_iTouchID)
		return false;

	if (m_pEntityToFind.IsValid())
	{
	  CScratchNode* pScratch = dynamic_cast<CScratchNode*>(m_pEntityToFind.Get());
	  if (pScratch)
	    {
	      pScratch->DrawTouch(a_pTouch->getLocation());
	      return true;
	    }
	  
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(m_pEntityToFind.Get());
		if (!pEntity) return false;

		if (m_sListenEvent == "Touch")
		{
			// if listen to touch change the entity when leaving it
			Vec2 oTouchLocation = a_pTouch->getLocation();

            cocos2d::Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				TouchMoveIn(pEntity);
			}
			else
			{
				TouchMoveOut(pEntity);
			}
		}
		else if (m_sListenEvent == "Move")
		{
			MoveEntity(a_pTouch, pEntity);
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
        //a_pEntity->GetCocosEntity()->setZOrder(0);
        a_pEntity->GetCocosEntity()->setGlobalZOrder(0);
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



Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{

  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity && pEntity->GetCocosEntity() != nullptr)
  {

    // Check if the entity intersects the touch event
	  Vec2 oTouchLocation = m_pTouch->getStartLocation();
    cocos2d::Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
    if (oBoundingBox.containsPoint(oTouchLocation) && !pEntity->IsLocked() && pEntity->IsVisible())
    {
      CScratchNode* pScratch = dynamic_cast<CScratchNode*>(pEntity);
      if (pScratch)
	{
		if (pScratch->IsListeningTo("Touch"))
		{
			m_sListenEvent = "Touch";
		}
        pScratch->DrawTouch(oTouchLocation);
        m_pEntityToFind.Set(pScratch);
        return RESULT_PRUNE;
	}
      // if so and if listenning to touch/move, store the entity
		if (pEntity->EventIsDisabled("Touch"))
		{
			return RESULT_PRUNE;
		}
		else if (pEntity->IsListeningTo("Touch"))
		{

		  m_pEntityToFind.Set(pEntity);
		  m_sListenEvent = "Touch";

		  M_STATS_SCREEN.nbTouches++;
		  StartTouch(pEntity);

		  return RESULT_PRUNE;
      }
		else if (pEntity->EventIsDisabled("Move"))
		{
			return RESULT_PRUNE;
		}
	  else if (pEntity->IsListeningTo("Move"))
	  {

		  m_pEntityToFind.Set(pEntity);
		  m_sListenEvent = "Move";

		  M_STATS_SCREEN.nbMoves++;
		  StartMove(pEntity);
		  pEntity->Dispatch("Move");

		  return RESULT_PRUNE;
	  }
	  else if (pEntity->IsListeningTo("CountdownPressed"))
	  {
		  m_pEntityToFind.Set(pEntity);
		  m_sListenEvent = "CountdownPressed";
		  pEntity->Dispatch("CountdownPressed");
		  StartTouch(pEntity);
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
    //a_pEntity->GetCocosEntity()->setZOrder(1);
    a_pEntity->GetCocosEntity()->setGlobalZOrder(1);
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


//void CTouchBeganVisitor::AnchorEntity(CEntityNode* a_pAnchoredEntity, CEntityNode* a_pAnchor)
//{
//
//	a_pAnchor->Dispatch("Anchor", a_pAnchoredEntity);
//	a_pAnchoredEntity->Dispatch("Anchored", a_pAnchor);
//}

} // namespace LM
