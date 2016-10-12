#ifndef _CFINDENTITIESVISITOR_H_
#define _CFINDENTITIESVISITOR_H_

#include "CVisitor.h"
#include "CEntityNode.h"

namespace LM
{

class CDispatchEventVisitor : public CVisitor
{
 protected:
  std::string m_sEvent;
  std::vector<CEntityNode*> m_vFoundEntities;

 public:
  CDispatchEventVisitor(const std::string& a_rEvent);
  
 protected:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  
};

}

#endif /* _CFINDENTITIESVISITOR_H_ */
