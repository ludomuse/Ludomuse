#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"


using namespace cocos2d;


namespace LM
{

CTouchBeganVisitor::CTouchBeganVisitor(Touch* a_pTouch, Event* a_pEvent) :
    m_pTouch(a_pTouch),
    m_pEvent(a_pEvent)
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
      // TODO : traverse in decreasing order of z-index
      for (CNode* itNode : *a_pNode)
      {
        Traverse(itNode);
      }
    }
    else
    {
      Traverse(a_pNode->GetCurrentNode());
    }

    ProcessNodeBottomUp(a_pNode);
  }
}


Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);

  if (pEntity)
  {
    if (pEntity->IsListeningTo("Touch"))
    {
		Vec2 oTouchLocation = m_pTouch->getLocationInView();
		Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
		if (oBoundingBox.containsPoint(oTouchLocation))
		{
			// if the Touch event intersects this element then dispatch event
			pEntity->Dispatch("Touch");
			return RESULT_PRUNE;
		}
    }
  }
  
  return RESULT_CONTINUE;
}


} // namespace LM
