#include "../Include/CFindTeamNodeIDVisitor.h"

namespace LM
{


CFindTeamNodeIDVisitor::CFindTeamNodeIDVisitor(const Desc<CNode>& a_pNode, const std::string& a_rID) :
    CFindEntityFromIDVisitor(a_pNode, a_rID)
{
  
}


void CFindTeamNodeIDVisitor::Traverse(CNode* a_pNode)
{
  CVisitor::Traverse(a_pNode);
}


} // namespace LM
