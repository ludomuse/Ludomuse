#ifndef _CTRANSITIONVISITOR_H_
#define _CTRANSITIONVISITOR_H_

#include "CVisitor.h"

namespace LM
{

class CTransitionVisitor : public CVisitor
{
 public:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;

};


} // namespace LM

#endif /* _CTRANSITIONVISITOR_H_ */
