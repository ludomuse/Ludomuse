#include "../Include/CValidateSceneVisitor.h"
#include "../Include/CEntityNode.h"
#include "../Include/CMenuNode.h"
#include "../Include/CSceneNode.h"
#include "../Include/CValidator.h"

namespace LM
{


CValidateSceneVisitor::CValidateSceneVisitor(const SEvent& a_rEvent) :
    m_bValidate(a_rEvent.m_bBoolValue),
	m_pSender(a_rEvent.m_pSender)
{
  
}


void CValidateSceneVisitor::Traverse(CNode* a_pNode)
{
	if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
	{
		CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
		CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
		CValidator* pValidator = dynamic_cast<CValidator*>(a_pNode);
		if (pEntity || pScene || pValidator)
		{
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


Result CValidateSceneVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
    if (pEntity == m_pSender)
    {
      if (m_bValidate)
      {
        // TODO : change button color etc
      }
      else
      {
        // change another way
      }
    }
    else if (pEntity->IsListeningTo("Validate") && m_bValidate)
    {
      // CMenuNode* pMenuNode = dynamic_cast<CMenuNode*>(pEntity); 
		CCLOG("CValidateSceneVisitor::ProcessNodeTopDown %s", pEntity->GetID().c_str());
      pEntity->Dispatch("Validate");
      return RESULT_PRUNE;
    }
  }
  return RESULT_CONTINUE;
}


} // namespace LM
