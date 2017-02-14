#include "../Include/CFindTeamNodeVisitor.h"

namespace LM
{


CFindTeamNodeVisitor::CFindTeamNodeVisitor(const Desc<CNode>& a_pNode) : CFindEntityFromTypeVisitor<CTeamNode>(a_pNode)
{
  
}


void CFindTeamNodeVisitor::Traverse(CNode* a_pNode)
{
  CVisitor::Traverse(a_pNode);
}


} // namespace LM
