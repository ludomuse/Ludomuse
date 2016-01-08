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

  /// \brief Forward the OnTouchBegan event to every child node 
  virtual bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent) override;

  virtual void Finish() override;
};


} // namespace LM

#endif /* _CPARALLELNODE_H_ */
