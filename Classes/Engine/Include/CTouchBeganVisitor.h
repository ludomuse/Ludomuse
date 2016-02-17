#ifndef _CTOUCHBEGANVISITOR_H_
#define _CTOUCHBEGANVISITOR_H_

#include "CVisitor.h"

namespace LM
{

class CTouchBeganVisitor : public CVisitor
{  

 public:
  CTouchBeganVisitor();


  virtual void Traverse(CNode* a_pNode);
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  
  
};

} // namespace LM

#endif /* _CTOUCHBEGANVISITOR_H_ */
