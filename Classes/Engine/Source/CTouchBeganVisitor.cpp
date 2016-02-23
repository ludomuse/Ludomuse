#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CSceneNode.h"
#include "../Include/CMenuNode.h"


using namespace cocos2d;


namespace LM
{

CTouchBeganVisitor::CTouchBeganVisitor(Touch* a_pTouch, Event* a_pEvent) :
    m_pTouch(a_pTouch),
    m_pEvent(a_pEvent),
	m_bStopVisiting(false),
	m_pTouchBeganEntity(nullptr)
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

	ProcessNodeBottomUp(a_pNode);

  }
}


bool CTouchBeganVisitor::OnTouchEnd(Touch* a_pTouch, Event* a_pEvent)
{
	if (m_pTouchBeganEntity)
	{
		if (m_sListenEvent == "Touch")
		{
			CCLOG("TouchEnd");
			Vec2 oTouchLocation = a_pTouch->getLocation();
			Rect oBoundingBox = m_pTouchBeganEntity->GetCocosEntity()->getBoundingBox();
			if (oBoundingBox.containsPoint(oTouchLocation))
			{
				m_pTouchBeganEntity->Dispatch(m_sListenEvent);
			}
		}
		else if (m_sListenEvent == "Move")
		{
			// Traverse the tree to find a drop area

			auto oMoveTo = MoveTo::create(0.5, m_oEntityPosition);
			m_pTouchBeganEntity->GetCocosEntity()->runAction(oMoveTo);
			
			auto oScaleTo = ScaleTo::create(0.5, m_fEntityScale);
			m_pTouchBeganEntity->GetCocosEntity()->runAction(oScaleTo);
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
    if (oBoundingBox.containsPoint(oTouchLocation))
    {
      // if so and if listenning to touch/move, store the entity
      if (pEntity->IsListeningTo("Touch"))
      {
		  m_pTouchBeganEntity = pEntity;
		  m_sListenEvent = "Touch";
		  return RESULT_PRUNE;
      }
	  else if (pEntity->IsListeningTo("Move"))
	  {
		  m_pTouchBeganEntity = pEntity;
		  m_sListenEvent = "Move";
		  m_oEntityPosition = m_pTouchBeganEntity->GetCocosEntity()->getPosition();
		  m_fEntityScale = m_pTouchBeganEntity->GetCocosEntity()->getScale();

		  auto oScaleTo1 = ScaleTo::create(0.05f, 2 * m_fEntityScale);
		  auto oScaleTo2 = ScaleTo::create(0.1f, 1.5 * m_fEntityScale);

		  auto oSequence = Sequence::create(oScaleTo1, oScaleTo2, nullptr);

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
	CMenuNode* pMenuNode = dynamic_cast<CMenuNode*>(a_pNode);
	// MenuNode gets a special treatment because it is not well managed by cocos
	// (GetCocosEntity does not return the right thing)
	if (pEntity && !pMenuNode)
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
