#include "../Include/CFindSceneFromIDVisitor.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CFindSceneFromIDVisitor::CFindSceneFromIDVisitor(const std::string& a_rSceneID, CKernel* a_pKernel):
    m_sSceneID(a_rSceneID),
    m_pKernel(a_pKernel),
    m_pScene(nullptr)
{

}


Result CFindSceneFromIDVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pSceneNode = dynamic_cast<CSceneNode*>(a_pNode);
  if (pSceneNode)
  {
    if (pSceneNode->GetSceneID() == m_sSceneID)
    {
      m_pScene = pSceneNode;
      return RESULT_PRUNE;
    }
  }
  return RESULT_CONTINUE;
}

CSceneNode* CFindSceneFromIDVisitor::GetSceneFound()
{
    return this->m_pScene;
}

bool CFindSceneFromIDVisitor::SceneFound()
{
    if(this->m_pScene != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}


}// End namespace LM

