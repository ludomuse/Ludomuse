#ifndef _CTOUCHBEGANVISITOR_H_
#define _CTOUCHBEGANVISITOR_H_

#include "CVisitor.h"
#include "CEntityNode.h"

#include "cocos2d.h"

namespace LM
{

class CTouchBeganVisitor : public CVisitor
{  
 private:
  cocos2d::Touch* m_pTouch;
  cocos2d::Event* m_pEvent;

  CEntityNode* m_pTouchBeganEntity;
  std::string m_sListenEvent;

  bool m_bStopVisiting;
  
 public:
  CTouchBeganVisitor(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);


  virtual void Traverse(CNode* a_pNode);


  // callbacks
  bool OnTouchEnd(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
  bool OnTouchMove(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

private:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  virtual Result ProcessNodeBottomUp(CNode* a_pNode) override;
  
  
};

} // namespace LM

#endif /* _CTOUCHBEGANVISITOR_H_ */
