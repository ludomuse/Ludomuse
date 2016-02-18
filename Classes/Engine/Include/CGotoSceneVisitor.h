#ifndef _CGOTOSCENEVISITOR_H_
#define _CGOTOSCENEVISITOR_H_

#include "CVisitor.h"

namespace LM
{

class CGotoSceneVisitor : public CVisitor
{
 private:
  std::string m_sSceneID;

 public:
  CGotoSceneVisitor(const std::string& a_sSceneID);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
};


} // namespace LM

#endif /* _CGOTOSCENEVISITOR_H_ */
