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

  CKernel* m_pKernel;

  CEntityNode* m_pTouchBeganEntity;
  cocos2d::Vec2 m_oEntityPosition;
  float m_fEntityScale;

  std::string m_sListenEvent;

  bool m_bStopVisiting;
  
 public:
  CTouchBeganVisitor(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent, CKernel* a_pKernel);


  virtual void Traverse(CNode* a_pNode);


  // callbacks
  bool OnTouchEnd(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
  bool OnTouchMove(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

protected:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  virtual Result ProcessNodeBottomUp(CNode* a_pNode) override;
  
  
};

} // namespace LM

#endif /* _CTOUCHBEGANVISITOR_H_ */
