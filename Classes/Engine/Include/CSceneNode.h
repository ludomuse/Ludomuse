#ifndef _CSCENE_H_
#define _CSCENE_H_

#include <string>

#include "CParallelNode.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

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

  std::string m_sRewardID;
  std::string m_sInitSound;
  std::string m_sValidSound;

public:
	bool m_bDashboardTrigger;


 public:
  CSceneNode(const std::string& a_rID = "none", CKernel* a_pKernel = nullptr);

  /// \returns the corresponding cocos2d scene
  cocos2d::Scene* GetScene();

  /// \returns the scene ID
  std::string GetID();
  
  /// \brief initialize the scene of the game
  cocos2d::Scene* CreateScene();

  virtual bool init();

  const std::string& GetSceneID() const;

  void SetSynced(bool a_bIsSynced);

  bool IsSynced();

  void DisplayDebugInfo();

  std::string GetSceneID() override;
#ifdef LUDOMUSE_EDITOR
  virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
  #endif
  void ToggleQuickBox();

  bool hasID(const std::string &a_rID);

  void SaveImage(std::function<void(cocos2d::RenderTexture*, const std::string&)> callback, float a_fScale = 1);

  std::string GetRewardID();
  void SetRewardID(std::string a_sRewardID);
  std::string GetInitSound();
  void SetInitSound(std::string a_sInitSound);
  std::string GetValidSound();
  void SetValidSound(std::string a_sValidSound);
#ifdef LUDOMUSE_EDITOR
  virtual bool UseFile(const std::string &a_sFilename) override;
#endif
  CREATE_FUNC(CSceneNode);
};

} // namespace LM

#endif /* _CSCENE_H_ */
