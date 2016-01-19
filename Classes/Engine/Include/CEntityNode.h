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

	static EAnchor IntToAnchor(int a_iIndex)
	{
		switch (a_iIndex)
		{
		case 0:
			return CENTER;
		case 1:
			return BOTTOM_LEFT;
		case 2:
			return LEFT;
		case 3:
			return TOP_LEFT;
		case 4:
			return TOP;
		case 5:
			return TOP_RIGHT;
		case 6:
			return RIGHT;
		case 7:
			return BOTTOM_RIGHT;
		case 8:
			return BOTTOM;
		default:
			return FLOAT;
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
