#ifndef _CENTITYNODE_H_
#define _CENTITYNODE_H_


#include "CNode.h"
#include "cocos2d.h"

namespace LM
{

/// \brief a class representing a physical entity in a scene
class CEntityNode : public CNode
{
 protected: // variables
  /// \brief the %width position on the screen
  int m_iXPosition;
  /// \brief the %height position on the screen 
  int m_iYPosition;

  cocos2d::Node* m_pCocosEntity;

 public:
  CEntityNode(int a_iXPosition, int a_iYPosition);
  
  /// \brief called when the scene is initialized
  virtual void Init() = 0;

  /// \returns the cocos2d corresponding entity
  virtual cocos2d::Node* GetCocosEntity();


 protected: // methods
  /// \brief must be called at the end of the Init overloaded
  ///        function to populate parent class in the tree 
  virtual void PopulateParent();

};

} // namespace LM

#endif /* _CENTITYNODE_H_ */
