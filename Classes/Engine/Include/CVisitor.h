#ifndef _CVISITOR_H_
#define _CVISITOR_H_


#include "CNode.h"

namespace LM
{

enum Result
{
  RESULT_CONTINUE,
  RESULT_PRUNE
};

class CVisitor
{
  CNode* m_pRootNode;
  CNode* m_pCurrentNode;

  CVisitor();
  virtual ~CVisitor();

  void Traverse(CNode* a_pNode);

  /// \brief Called when decending to a new Node. Recursion stops on RESULT_PRUNE
  virtual Result ProcessNodeTopDown(CNode* a_pNode);
  /// \brief Called after child node has been processed before going back to parent node
  virtual void ProcessNodeBottomUp(CNode* a_pNode);
  
};


} // namespace LM
#endif /* _CVISITOR_H_ */
