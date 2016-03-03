#ifndef _CENTITYNODE_H_
#define _CENTITYNODE_H_


#include "CNode.h"
#include "CCallback.h"
#include "cocos2d.h"

#include <map>

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

	/// \fn convert a_iIndex to its corresponding EAnchor entity
	static EAnchor IntToAnchor(int a_iIndex)
	{
		switch (a_iIndex)
		{
		case 0:
			return CENTER;
		case 1:
			return TOP_LEFT;
		case 2:
			return TOP;
		case 3:
			return TOP_RIGHT;
		case 4:
			return RIGHT;
		case 5:
			return BOTTOM_RIGHT;
		case 6:
			return BOTTOM;
		case 7:
			return BOTTOM_LEFT;
		case 8:
			return LEFT;
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

  /// \brief the width of the entity on the screen
  int m_iWidth;
  /// \brief the height of the entity on the screen
  int m_iHeight;

  cocos2d::Vec2 m_oEntityStartLocation;

  float m_fEntityStartScale;

  bool m_bVisible;

  bool m_bLocked;

  std::string m_sID;

  /// \brief the events the entity is listening to
  std::map<std::string, CEventCallback> m_mListeners;

  /// \brief the corresponging cocos2d entity 
  cocos2d::Node* m_pCocosEntity;

 public:
	 CEntityNode(EAnchor a_eAnchor = EAnchor::FLOAT,
		 int a_iWidth = 0,
		 int a_iHeight = 0,
		 int a_iXPosition = 0,
		 int a_iYPosition = 0,
		 const std::string& a_sID = ""
);
  
  /// \brief called when the scene is initialized
  //virtual void Init() = 0;
  virtual void UnInit() override;

  /// \returns the cocos2d corresponding entity
  virtual cocos2d::Node* GetCocosEntity();

  /// \brief subscribe the entity to a_rEvent
  void AddListener(const std::string& a_rEvent, const CEventCallback& a_rCallback);

  /// \brief checks if the entity is listening to 
  bool IsListeningTo(const std::string& a_rEvent);

  /// \brief dispatch this event to the entity
  void Dispatch(const std::string& a_rEvent);

  /// \brief set the visibility on StartUp 
  virtual void SetVisible(bool a_bVisible);

  virtual bool IsVisible();

  virtual std::string GetID();
  virtual void SetID(const std::string& a_rID);

  /// \change the visibility
  /// \param[in] a_bVisible true to show the item
  virtual void Show(bool a_bVisible = true);

  virtual bool IsLocked();

  virtual cocos2d::Vec2 GetEntityStartLocation();

  virtual float GetEntityStartScale();

  virtual void Fade();

  ////////////////////////// Static methods
  static bool Lock(CEntityNode* a_pEntity);

  static void Release(CEntityNode* a_pEntity);

 protected: // methods
  /// \brief must be called at the end of the Init overloaded
  ///        function to populate parent class in the tree
  /// \param[in] a_bDoScaling if the function must handle the scaling or not
  virtual void PopulateParent(bool a_bDoScaling = true);

  virtual cocos2d::Scene* GetParentScene();

  virtual cocos2d::Size GetParentVisibleSize();

  virtual cocos2d::Vec2 GetParentOrigin();

  virtual cocos2d::Size GetVisibleSize();

  virtual cocos2d::Vec2 GetOrigin();


};

} // namespace LM

#endif /* _CENTITYNODE_H_ */
