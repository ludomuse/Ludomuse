#ifndef _CSELECTORNODE_H_
#define _CSELECTORNODE_H_

#include "CNode.h"

namespace LM
{

class CSelectorNode : public CNode
{
 private:
  bool SelectCurrentNode(int a_iIndex);
};

} // namespace LM

#endif /* _CSELECTORNODE_H_ */
