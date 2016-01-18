#ifndef _CENTITYNODE_H_
#define _CENTITYNODE_H_


#include "CNode.h"
#include "cocos2d.h"

namespace LM
{

	enum EAnchor 
	{
		CENTER,
		BOTTOM_LEFT,
		LEFT, 
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		FLOAT
	};

	static void IntToAnchor(int a_iIndex)
	{
		switch (a_iIndex)
		{
		default:
			break;
		}
	};

/// \brief a class representing a physical entity in a scene
class CEntityNode : public CNode
{
 protected: // variables
  /// \brief where the entity is anchored
  EAnchor m_eAnchor;

  /// \brief the %width position on the screen
  int m_iXPosition;
  /// \brief the %height position on the screen 
  int m_iYPosition;

  cocos2d::Node* m_pCocosEntity;

 public:
  CEntityNode(EAnchor a_eAnchor = EAnchor::FLOAT, 
	          int a_iXPosition = 0, 
	          int a_iYPosition = 0);
  
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
