#ifndef _CSEQUENCENODE_H_
#define _CSEQUENCENODE_H_

#include "CNode.h"


namespace LM
{

class CSequenceNode : public CNode
{


 public:
  virtual bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent) override;
};

} // namespace LM

#endif /* _CSEQUENCENODE_H_ */
