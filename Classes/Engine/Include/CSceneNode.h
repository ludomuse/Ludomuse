#ifndef _CSCENE_H_
#define _CSCENE_H_

#include <string>

#include "CParallelNode.h"

namespace LM
{


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

  bool m_bDebugMode;

public:
	bool m_bDashboardTrigger;
  
 public:
  CSceneNode(std::string a_sID = "none", bool a_bDebugMode = false);

  /// \returns the corresponding cocos2d scene
  cocos2d::Scene* GetScene();
  
  /// \brief initialize the scene of the game
  cocos2d::Scene* CreateScene();

  virtual bool init();

  const std::string& GetSceneID() const;

  void SetSynced(bool a_bIsSynced);

  bool IsSynced();

  void DisplayDebugInfo();

  CREATE_FUNC(CSceneNode);
};

} // namespace LM

#endif /* _CSCENE_H_ */
