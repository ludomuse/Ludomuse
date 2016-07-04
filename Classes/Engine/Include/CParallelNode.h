#ifndef _CPARALLELNODE_H_
#define _CPARALLELNODE_H_

#include "../Include/CNode.h"


namespace LM
{

/// \class CParallelnode
/// \ingroup Engine
/// \brief A node executing its children in parallel 
class CParallelNode : public CNode
{
 private:
  int m_iFinishedNodesCount;

 public:
  CParallelNode();

  // TODO /// \brief Forward the OnTouchBegan event to every child node 
  //virtual bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent) override;
  /// \details Finish when all the children nodes are finished
  virtual void Finish() override;

  virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
};


} // namespace LM

#endif /* _CPARALLELNODE_H_ */
