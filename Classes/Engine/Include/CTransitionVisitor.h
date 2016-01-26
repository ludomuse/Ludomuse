#ifndef _CTRANSITIONVISITOR_H_
#define _CTRANSITIONVISITOR_H_

#include "CVisitor.h"

namespace LM
{

typedef cocos2d::TransitionScene* (*FPTransitionCreate)(float, cocos2d::Scene*);

class CTransitionVisitor : public CVisitor
{
private:
	bool m_bTransitionNext;

 public:
	 CTransitionVisitor(bool a_bTransitionNext);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;

};


} // namespace LM

#endif /* _CTRANSITIONVISITOR_H_ */
