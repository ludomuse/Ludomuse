#ifndef _CPARALLELNODE_H_
#define _CPARALLELNODE_H_

#include "../Include/CNode.h"


namespace LM
{

class CParallelNode : public CNode
{
  

 public:
  CParallelNode();

  /**
   * Forward the OnTouchBegan event to every child node 
   */
  virtual bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
};


} // namespace LM

#endif /* _CPARALLELNODE_H_ */
