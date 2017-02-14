#ifndef _CFINDTEAMNODEIDVISITOR_H_
#define _CFINDTEAMNODEIDVISITOR_H_

#include "CFindEntityFromIDVisitor.h"
#include "CTeamNode.h"

namespace LM
{

class CFindTeamNodeIDVisitor: public CFindEntityFromIDVisitor
{
 public:
	 CFindTeamNodeIDVisitor(const Desc<CNode>& a_pNode, const std::string& a_rID);

  virtual void Traverse(CNode* a_pNode) override;

};

} // namespace LM

#endif /* _CFINDTEAMNODEIDVISITOR_H_ */
