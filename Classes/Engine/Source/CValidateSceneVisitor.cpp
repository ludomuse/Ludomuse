#include "../Include/CValidateSceneVisitor.h"
#include "../Include/CEntityNode.h"
#include "../Include/CMenuNode.h"

namespace LM
{


SValidateSceneArgs::SValidateSceneArgs(bool a_bValidate, CNode* a_pSender) :
    bValidate(a_bValidate),
    pSender(a_pSender)
{
  
}


CValidateSceneVisitor::CValidateSceneVisitor(SValidateSceneArgs a_oArgs) :
    m_oArgs(a_oArgs)
{
  
}


Result CValidateSceneVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
    if (pEntity == m_oArgs.pSender)
    {
      if (m_oArgs.bValidate)
      {
        // TODO : change button color etc
      }
      else
      {
        // change another way
      }
    }
    else if (pEntity->IsListeningTo("Validate"))
    {
      // CMenuNode* pMenuNode = dynamic_cast<CMenuNode*>(pEntity);      
      pEntity->Dispatch("Validate");
      return RESULT_PRUNE;
    }
  }
  return RESULT_CONTINUE;
}


} // namespace LM
