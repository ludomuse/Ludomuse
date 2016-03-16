#ifndef _CFINDENTITYFROMTYPE_H_
#define _CFINDENTITYFROMTYPE_H_

#include "CFindEntityVisitor.h"

namespace LM
{

template<typename T>
class CFindEntityFromTypeVisitor : public CFindEntityVisitor
{
 public:
  CFindEntityFromTypeVisitor(const Desc<CNode>& a_pNode) : CFindEntityVisitor(a_pNode, "")
  {
  }

  Result ProcessNodeTopDown(CNode* a_pNode) override
  {
    T* pEntity = dynamic_cast<T*>(a_pNode);
    if (pEntity)
    {
      m_pEntityToFind.Set(pEntity);
      return RESULT_PRUNE;
    }
	return RESULT_CONTINUE;
  }

};

} // namespace LM

#endif /* _CFINDENTITYFROMTYPE_H_ */
