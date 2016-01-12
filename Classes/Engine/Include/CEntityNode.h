#ifndef _CENTITYNODE_H_
#define _CENTITYNODE_H_


#include "CNode.h"

namespace LM
{

/// \brief a class representing a physical entity in a scene
class CEntityNode : public CNode
{
 protected:
  /// \brief the %width position on the screen
  int m_iXPosition;
  /// \brief the %height position on the screen 
  int m_iYPosition;

 public:
  CEntityNode(int a_iXPosition, int a_iYPosition);
  
  /// \brief called when the scene is initialized
  virtual void Init() = 0;
};

} // namespace LM

#endif /* _CENTITYNODE_H_ */
