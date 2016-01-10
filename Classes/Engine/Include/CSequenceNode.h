#ifndef _CSEQUENCENODE_H_
#define _CSEQUENCENODE_H_

#include "CNode.h"


namespace LM
{


/// \class CSequenceNode
/// \ingroup Engine
/// \brief a node executing its children in sequence
class CSequenceNode : public CNode
{


 public:
  /// \brief forwards the OnTouchBegan event to the currently active child in the sequence
  virtual bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent) override;

  /// \details calling it will increment the current node id
  ///          and the node will finish when each node in the
  ///          sequence has finished
  virtual void Finish() override;
};

} // namespace LM

#endif /* _CSEQUENCENODE_H_ */
