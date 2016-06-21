#ifndef _VALIDATESCENEVISITOR_H_
#define _VALIDATESCENEVISITOR_H_

#include "CVisitor.h"

#include "CCallback.h"

namespace LM
{

class CValidateSceneVisitor : public CVisitor
{

 private:
	 bool m_bValidate;
	 CNode* m_pSender;

 public:
  CValidateSceneVisitor(const SEvent& a_rEvent);

  virtual void Traverse(CNode* a_pNode);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  
};

} // namespace LM

#endif /* _VALIDATESCENEVISITOR_H_ */
