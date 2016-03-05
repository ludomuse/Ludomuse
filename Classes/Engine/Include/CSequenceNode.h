#ifndef _CSEQUENCENODE_H_
#define _CSEQUENCENODE_H_

#include "CNode.h"


namespace LM
{


/// \class CSequenceNode
/// \ingroup Engine
/// \brief a node executing its children in sequence
class CSequenceNode : public CNode
{


 public:


  /// \details calling it will increment the current node id
  ///          and the node will finish when each node in the
  ///          sequence has finished
  virtual void Finish() override;


};

} // namespace LM

#endif /* _CSEQUENCENODE_H_ */
