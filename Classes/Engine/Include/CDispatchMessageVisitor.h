#ifndef _CDISPATCHMESSAGEVISITOR_H_
#define _CDISPATCHMESSAGEVISITOR_H_

#include "CVisitor.h"

namespace LM
{

class CDispatchMessageVisitor : public CVisitor
{
 private:
  std::vector<std::string> m_vSplittedMessage;
  std::string m_sMessage;

 public:
  CDispatchMessageVisitor(const std::string& a_rMessage);

  virtual Result ProcessNodeTopDown(CNode* a_pNode);
};

} // namespace LM

#endif /* _CDISPATCHMESSAGEVISITOR_H_ */
