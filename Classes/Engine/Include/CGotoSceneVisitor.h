#ifndef _CGOTOSCENEVISITOR_H_
#define _CGOTOSCENEVISITOR_H_

#include "CVisitor.h"
#include "CKernel.h"

namespace LM
{

class CGotoSceneVisitor : public CVisitor
{
 private:
  std::string m_sSceneID;
  CKernel* m_pKernel;

 public:
  CGotoSceneVisitor(const std::string& a_rSceneID, CKernel* a_pKernel);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
};


} // namespace LM

#endif /* _CGOTOSCENEVISITOR_H_ */
