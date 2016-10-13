#include "../Include/CDispatchEventVisitor.h"
#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{

CDispatchEventVisitor::CDispatchEventVisitor(const std::string& a_rEvent):
    m_sEvent(a_rEvent)
{
  
}


Result CDispatchEventVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
    if (pEntity->IsListeningTo(m_sEvent))
    {
      pEntity->Dispatch(m_sEvent);
    }
  }

  return RESULT_CONTINUE;
}


} // namespace LM
