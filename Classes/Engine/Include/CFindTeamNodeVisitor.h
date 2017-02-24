#ifndef _CFINDTEAMNODEVISITOR_H_
#define _CFINDTEAMNODEVISITOR_H_

#include "CFindEntityFromTypeVisitor.h"
#include "CTeamNode.h"

namespace LM
{

class CFindTeamNodeVisitor: public CFindEntityFromTypeVisitor<CTeamNode>
{
 public:
  CFindTeamNodeVisitor(const Desc<CNode>& a_pNode);

  virtual void Traverse(CNode* a_pNode) override;

};

} // namespace LM

#endif /* _CFINDTEAMNODEVISITOR_H_ */
