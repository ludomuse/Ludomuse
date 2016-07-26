#include "../Include/CParallelNode.h"


using namespace cocos2d;

namespace LM
{


CParallelNode::CParallelNode() : m_iFinishedNodesCount(0)
{
  
}

// TODO
//bool CParallelNode::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
//{
//  bool bSuccess = true;
//  CNode::Iterator itChild;
//  // forward the OnTouchBegan event to each child
//  //DoForEachChildNode([&bSuccess, &a_pTouch, &a_pEvent](CNode* a_pChild) {
//	 // bSuccess = bSuccess && a_pChild->OnTouchBegan(a_pTouch, a_pEvent);
//  //});
//  for (CNode* itChild : *this)
//  {
//	  bSuccess = bSuccess && itChild->OnTouchBegan(a_pTouch, a_pEvent);
//  }
//  // if each child's OnTouchbegan succeeds then success
//  return bSuccess;
//}



void CParallelNode::Finish()
{
  // if all the children nodes are finished then finish the parallel node
  m_iFinishedNodesCount++;
  if (m_iFinishedNodesCount >= m_vChildren.size())
  {
    m_pParent->Finish();
  }
}


void CParallelNode::ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
{

}

bool CParallelNode::hasID(const std::string& a_rID)
{
    return false;
}

} // namespace LM
