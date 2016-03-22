#ifndef _CTOUCHBEGANVISITOR_H_
#define _CTOUCHBEGANVISITOR_H_

#include "CFindEntityTouchVisitor.h"
#include "CEntityNode.h"

#include "cocos2d.h"


namespace LM
{



class CTouchBeganVisitor : public CFindEntityTouchVisitor
{  
 private:
  cocos2d::Event* m_pEvent;

  CKernel* m_pKernel;

  std::string m_sListenEvent;

  bool m_bStopVisiting;
  
 public:
  CTouchBeganVisitor(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent, CKernel* a_pKernel);

  // callbacks
  bool OnTouchEnd(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
  bool OnTouchMove(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

protected:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  

  // entities actions
  virtual void MoveEntity(cocos2d::Touch* a_pTouch, CEntityNode* a_pEntity);
  virtual void TouchMoveOut(CEntityNode* a_pEntity);
  virtual void TouchMoveIn(CEntityNode* a_pEntity);
  virtual void TouchStop(CEntityNode* a_pEntity);
  virtual void MoveEntityBack(CEntityNode* a_pEntity);
  virtual void StartTouch(CEntityNode* a_pEntity);
  virtual void StartMove(CEntityNode* a_pEntity);
  //virtual void AnchorEntity(CEntityNode* a_pAnchoredEntity, CEntityNode* a_pAnchor);
};

} // namespace LM

#endif /* _CTOUCHBEGANVISITOR_H_ */
