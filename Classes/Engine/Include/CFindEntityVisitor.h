#ifndef _CFINDENTITYVISITOR_H_
#define _CFINDENTITYVISITOR_H_

#include "CEntityNode.h"
#include "CVisitor.h"
#include "cocos2d.h"


namespace LM
{

class CFindEntityVisitor : public CVisitor
{
 private:
  cocos2d::Touch* m_pTouch;
  std::string m_sEvent;
  CEntityNode** m_pEntityToFind;

  bool m_bStopVisiting;

 public:
  CFindEntityVisitor(cocos2d::Touch* a_pTouch, CEntityNode** a_pEntity,
                     const std::string& a_sEvent);


  virtual void Traverse(CNode* a_pNode) override;

 protected:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  virtual Result ProcessNodeBottomUp(CNode* a_pNode) override;
  
};

} // namespace LM

#endif /* _CFINDENTITYVISITOR_H_ */
