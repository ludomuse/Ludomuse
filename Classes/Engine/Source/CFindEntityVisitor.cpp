#include "../Include/CFindEntityVisitor.h"
#include "../Include/CSceneNode.h"

using namespace cocos2d;

namespace LM
{


CFindEntityVisitor::CFindEntityVisitor(const Desc<CEntityNode>& a_pEntity, const std::string& a_sEvent) :
	m_sEvent(a_sEvent),
	m_pEntityToFind(a_pEntity),
	m_bStopVisiting(false)
{
}



void CFindEntityVisitor::Traverse(CNode* a_pNode)
{
  if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
  {
    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
    CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
    if (pEntity || pScene)
    {
      // traverse in decreasing order of z-index
      const std::vector<CNode*>& oNodes = a_pNode->GetChildren();
       for (int i = oNodes.size() - 1; i >= 0; --i)
       {
         // stop traversing the tree
         if (m_bStopVisiting) return;
			
         Traverse(oNodes[i]);
       }
      //std::vector<CNode*>::reverse_iterator itNode;
      //for (itNode = oNodes.rbegin(); itNode != oNodes.rend(); ++itNode)
      //{
      //  // stop traversing the tree
      //  if (m_bStopVisiting) return;

      //  Traverse(*itNode);
      //}
      
    }
    else
    {
      Traverse(a_pNode->GetCurrentNode());
    }

  }
  ProcessNodeBottomUp(a_pNode);

}


Result CFindEntityVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
	CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
	if (pEntity)
	{
		
		if (pEntity->IsListeningTo(m_sEvent))
		{
			m_pEntityToFind.Set(pEntity);
			return RESULT_PRUNE;
		}
	}

	return RESULT_CONTINUE;
}


Result CFindEntityVisitor::ProcessNodeBottomUp(CNode* a_pNode)
{

  if (m_pEntityToFind.IsValid())
  {
    m_bStopVisiting = true;
    return RESULT_STOP;
  }

  return RESULT_CONTINUE;
}

} // namespace LM
