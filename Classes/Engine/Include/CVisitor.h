#ifndef _CVISITOR_H_
#define _CVISITOR_H_


#include "CNode.h"

namespace LM
{

/// \brief The result of the traversal of a node
enum Result
{
  RESULT_CONTINUE,
  RESULT_PRUNE,
  RESULT_STOP
};

class CVisitor
{
protected:
  /// \brief the root node of the traversal
  CNode* m_pRootNode;
  /// \brief the current node of the traversal
  CNode* m_pCurrentNode;

  
public:
  CVisitor();
  virtual ~CVisitor();

  /// \brief Traverse the node to visit it
  virtual void Traverse(CNode* a_pNode);

  /// \brief Called when decending to a new Node. Recursion stops on RESULT_PRUNE
  virtual Result ProcessNodeTopDown(CNode* a_pNode);
  /// \brief Called after child node has been processed before going back to parent node
  virtual Result ProcessNodeBottomUp(CNode* a_pNode);
  
};


} // namespace LM
#endif /* _CVISITOR_H_ */
