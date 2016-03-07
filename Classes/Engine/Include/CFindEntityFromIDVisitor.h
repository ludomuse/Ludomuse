#ifndef _CFINDENTITYFROMIDVISITOR_H_
#define _CFINDENTITYFROMIDVISITOR_H_

#include "CFindEntityVisitor.h"

namespace LM
{

class CFindEntityFromIDVisitor : public CFindEntityVisitor
{
 private:
  std::string m_sID;
  
 public:
  CFindEntityFromIDVisitor(const Desc<CEntityNode>& a_pEntity, const std::string& a_sID);
  virtual Result ProcessNodeTopDown(CNode* a_pNode);
};

} // namespace LM

#endif /* _CFINDENTITYFROMIDVISITOR_H_ */
