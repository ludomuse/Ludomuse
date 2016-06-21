#include "../Include/CDispatchMessageVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CEntityNode.h"

#include "../../Modules/Util/Include/Util.h"

namespace LM
{


CDispatchMessageVisitor::CDispatchMessageVisitor(const std::string& a_rMessage) :
    m_sMessage(a_rMessage)
{

	m_vSplittedMessage = StringSplit(a_rMessage);
}


void CDispatchMessageVisitor::Traverse(CNode* a_pNode)
{
	if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
	{
		/*CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
		CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
		if (pEntity || pScene)
		{*/
			for (CNode* itNode : *a_pNode)
			{
				Traverse(itNode);
			}
		/*}
		else
		{
			CNode* pTraverse = a_pNode->GetCurrentNode();
			if (pTraverse)
				Traverse(pTraverse);
		}*/

	}
	ProcessNodeBottomUp(a_pNode);

}


Result CDispatchMessageVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
  if (pScene)
  {
    if (m_vSplittedMessage.size() > 1 && pScene->GetSceneID() != m_vSplittedMessage[0])
    {
      return RESULT_PRUNE;
    }
    return RESULT_CONTINUE;
  }

  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
	  if (m_vSplittedMessage.size() > 2)
	  {
		  if (pEntity->IsListeningTo(m_vSplittedMessage[1]) &&
			  pEntity->GetID() == m_vSplittedMessage[2])
		  {
			  pEntity->Dispatch(m_vSplittedMessage[1]);
			  return RESULT_PRUNE;
		  }
		  else if (pEntity->EventIsDisabled(m_vSplittedMessage[1]))
		  {
			  return RESULT_PRUNE;
		  }
	  }
	  else if (m_vSplittedMessage.size() == 1)
	  {
		  if (pEntity->IsListeningTo(m_vSplittedMessage[0]))
		  {
			  pEntity->Dispatch(m_vSplittedMessage[0]);
			  return RESULT_PRUNE;
		  }
		  else if (pEntity->EventIsDisabled(m_vSplittedMessage[0]))
		  {
			  return RESULT_PRUNE;
		  }
	  }
  }
  return RESULT_CONTINUE;
}


} // namespace LM
