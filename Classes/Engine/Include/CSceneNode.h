#ifndef _CSCENE_H_
#define _CSCENE_H_

#include <string>

#include "CParallelNode.h"
#include "ui/CocosGUI.h"

namespace LM
{

	class CKernel;

/// \class CSceneNode
/// \ingroup  Engine
/// \brief a node representing a SCene in the game
class CSceneNode : public CParallelNode, public cocos2d::Layer
{
 private:
  /// \brief the corresponding cocos scene
  cocos2d::Scene* m_pScene;

  /// \brief The scene unique ID
  std::string m_sID;

  bool m_bIsSynced;

  CKernel* m_pKernel;
  cocos2d::ui::EditBox* m_pQuickBox;
  cocos2d::MenuItemImage* m_pQuickButton;

public:
	bool m_bDashboardTrigger;


 public:
  CSceneNode(std::string a_sID = "none", CKernel* a_pKernel = nullptr);

  cocos2d::Image *getPreview();


  /// \returns the corresponding cocos2d scene
  cocos2d::Scene* GetScene();
  
  /// \brief initialize the scene of the game
  cocos2d::Scene* CreateScene();

  virtual bool init();

  const std::string& GetSceneID() const;

  void SetSynced(bool a_bIsSynced);

  bool IsSynced();

  void DisplayDebugInfo();

  virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
  void ToggleQuickBox();

  bool hasID(const std::string &a_rID);

  CREATE_FUNC(CSceneNode);
};

} // namespace LM

#endif /* _CSCENE_H_ */
