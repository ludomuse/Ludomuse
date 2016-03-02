#include "../Include/CFindEntityTouchVisitor.h"

using namespace cocos2d;

namespace LM
{


CFindEntityTouchVisitor::CFindEntityTouchVisitor(Touch* a_pTouch, const Desc<CEntityNode>& a_pEntity,
                                       const std::string& a_sEvent) :
	CFindEntityVisitor(a_pEntity, a_sEvent),
    m_pTouch(a_pTouch)
{
  
}


Result CFindEntityTouchVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
    Vec2 oTouchLocation = m_pTouch->getLocation();
    Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();

    if (oBoundingBox.containsPoint(oTouchLocation) && pEntity->IsVisible())
    {
      if (pEntity->IsListeningTo(m_sEvent))
      {
        m_pEntityToFind.Set(pEntity);
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



} // namespace LM
