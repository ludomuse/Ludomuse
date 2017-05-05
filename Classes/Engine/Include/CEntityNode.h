#ifndef _CENTITYNODE_H_
#define _CENTITYNODE_H_


#include "CNode.h"
#include "CCallback.h"
#include "cocos2d.h"
#include "CSceneNode.h"

#include <map>

namespace LM
{

	enum EAnchor 
	{
        CENTER = 0,
        BOTTOM_LEFT = 7,
        LEFT = 8,
        TOP_LEFT = 1,
        TOP = 2,
        TOP_RIGHT = 3,
        RIGHT = 4,
        BOTTOM_RIGHT = 5,
        BOTTOM = 6,
        FLOAT = 9
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

  bool m_bColored;

  bool m_bLocked;

  std::string m_sID;

  /// \brief the events the entity is listening to
  std::map<std::string, std::vector<CEventCallback>> m_mListeners;

  std::set<std::string> m_oDisabledEvents;

  /// \brief the corresponging cocos2d entity 
  cocos2d::Node* m_pCocosEntity;

 public:
	 CEntityNode(EAnchor a_eAnchor = EAnchor::FLOAT,
		 int a_iWidth = 0,
		 int a_iHeight = 0,
		 int a_iXPosition = 0,
		 int a_iYPosition = 0,
		 const std::string& a_sID = "");

  virtual bool hasID(const std::string& a_sID);


  /// \brief called when the scene is initialized
  //virtual void Init() = 0;
  virtual void UnInit(bool removeChild = true) override;

  virtual void Revert(bool a_bVisible = false);

  /// \returns the cocos2d corresponding entity
  virtual cocos2d::Node* GetCocosEntity();

  /// \brief subscribe the entity to a_rEvent
  void AddListener(const std::string& a_rEvent, const CEventCallback& a_rCallback);

  std::pair<std::string,CEventCallback>* GetCallback(const std::string& a_sCallbackName);
  void RemoveCallbacks(const std::string& a_sCallbackName);

  void DisableEvent(const std::string& a_rEvent);

  void EnableEvent(const std::string& a_rEvent);

  bool EventIsDisabled(const std::string& a_rEvent);

  /// \brief checks if the entity is listening to 
  bool IsListeningTo(const std::string& a_rEvent);

  /// \brief dispatch this event to the entity
  void Dispatch(const std::string& a_rEvent, CEntityNode* a_pSender = nullptr);

  /// \brief set the visibility on StartUp 
  virtual void SetVisible(bool a_bVisible);

  virtual bool IsVisible();

  virtual std::string GetID();
  virtual void SetID(const std::string& a_rID);

  /// \brief return the first non blank ID in entity node upper hierarchy
  virtual std::string GetHierarchyID();

  /// \change the visibility
  /// \param[in] a_bVisible true to show the item
  virtual void Show(bool a_bVisible = true);

  /// \add color to a darken node
  /// \param[in] a_bColorize true to enable the colors
  virtual void Colorize(bool a_bColored = true);

  virtual bool IsLocked();

  virtual cocos2d::Vec2 GetEntityStartLocation();

  virtual float GetEntityStartScale();

  virtual void Fade();

  virtual void FadeIn();

  virtual void ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator);

  virtual void ToJsonListener(rapidjson::Value& a_rListeners, rapidjson::Document::AllocatorType& a_rAllocator);

  int GetWidth();

  int GetHeight();

  void SetWidth(int a_iWidth, bool a_bUpdate = true);

  void SetHeight(int a_iHeight, bool a_bUpdate = true);

  void ChangeAnchor(int a_anchor);

  int GetAnchor();

  bool IsMovable();

  ////////////////////////// Static methods
  static bool Lock(CEntityNode* a_pEntity);

  static void Release(CEntityNode* a_pEntity);

  virtual CSceneNode* GetParentSceneNode();

  virtual void Copy(CEntityNode *a_sNode, bool a_bRecCopy = true);
  virtual bool UseFile(const std::string &a_sFilename);
  virtual bool UseFileInCallbacks(const std::string &a_sFilename);

  virtual std::string GetSceneID() override;

 protected: // methods
  /// \brief must be called at the end of the Init overloaded
  ///        function to populate parent class in the tree
  /// \param[in] a_bDoScaling if the function must handle the scaling or not
  virtual void PopulateParent(bool a_bDoScaling = true, bool a_bAddToParent = true);

  virtual cocos2d::Scene* GetParentScene();

  virtual cocos2d::Size GetParentVisibleSize();

  virtual cocos2d::Vec2 GetParentOrigin();

  virtual cocos2d::Size GetVisibleSize();

  virtual cocos2d::Vec2 GetOrigin();

//  void ChangeWidth(int a_iWidth);

//  void ChangeHeight(int a_iHeight);

  virtual void Update();

};

} // namespace LM

#endif /* _CENTITYNODE_H_ */
