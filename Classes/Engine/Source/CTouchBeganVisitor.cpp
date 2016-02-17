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
		std::vector<CNode*> oNodes = a_pNode->GetChildren();
		//std::sort(oNodes.begin(), oNodes.end(), [](CNode* pLeftNode, CNode* pRightNode) {
		//	CEntityNode* pLeftEntity = dynamic_cast<CEntityNode*>(pLeftNode);
		//	CEntityNode* pRightEntity = dynamic_cast<CEntityNode*>(pRightNode);
		//	if (pRightEntity && pLeftEntity)
		//	{
		//		int iLeftOrder = pLeftEntity->GetCocosEntity()->getLocalZOrder();
		//		int iRightOrder = pRightEntity->GetCocosEntity()->getLocalZOrder();
		//		return  iLeftOrder > iRightOrder;					
		//	}
		//	return true;
		//});
		for (int i = oNodes.size() - 1; i >= 0; --i)
		{
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


Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);

  if (pEntity)
  {

    // Check if the entity intersects the touch event
    Vec2 oTouchLocation = m_pTouch->getLocationInView();
    Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
    if (oBoundingBox.containsPoint(oTouchLocation))
    {
      // if so and if listenning to touch, dispatch the event to the entity
      if (pEntity->IsListeningTo("Touch"))
      {
        pEntity->Dispatch("Touch");
		return RESULT_PRUNE;
      }
      
    }
  }
  
  return RESULT_CONTINUE;
}


} // namespace LM
