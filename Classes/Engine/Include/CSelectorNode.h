#ifndef _CSELECTORNODE_H_
#define _CSELECTORNODE_H_

#include "CNode.h"

namespace LM
{

class CSelectorNode : public CNode
{
 private:
  /// \brief select the current node to execute
  bool SelectCurrentNode(int a_iIndex);

  // the Finish method of the SelectorNode is the default finish method
  // ie finish when the selected node finishes
};

} // namespace LM

#endif /* _CSELECTORNODE_H_ */
