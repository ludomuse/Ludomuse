#include "../Include/CDispatchMessageVisitor.h"
#include "../Include/CSceneNode.h"

namespace LM
{


CDispatchMessageVisitor::CDispatchMessageVisitor(const std::string& a_rMessage) :
    m_sMessage(a_rMessage)
{

  // split string
  int iFoundFirst = 0;
  int iFoundNext = m_sMessage.find_first_of(':');
  while (iFoundNext != std::string::npos)
  {
    m_vSplittedMessage.push_back(m_sMessage.substr(iFoundFirst, iFoundNext));
    iFoundFirst = iFoundNext + 1;
    iFoundNext = m_sMessage.find_first_of(':', iFoundFirst);
  }
  m_vSplittedMessage.push_back(m_sMessage.substr(iFoundFirst, m_sMessage.size()));
}



Result CDispatchMessageVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
  if (pScene)
  {
    if (pScene->GetSceneID() != m_vSplittedMessage[0])
    {
      return RESULT_PRUNE;
    }

    
  }
}


} // namespace LM
