#ifndef _VALIDATESCENEVISITOR_H_
#define _VALIDATESCENEVISITOR_H_

#include "CVisitor.h"

namespace LM
{

struct SValidateSceneArgs
{
  bool bValidate;
  CNode* pSender;

  SValidateSceneArgs(bool a_bValidate, CNode* a_pSender);
};

class CValidateSceneVisitor : public CVisitor
{

 private:
  SValidateSceneArgs m_oArgs;

 public:
  CValidateSceneVisitor(SValidateSceneArgs a_oArgs);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  
};

} // namespace LM

#endif /* _VALIDATESCENEVISITOR_H_ */
