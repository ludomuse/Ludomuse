#ifndef _CSCENE_H_
#define _CSCENE_H_

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
  
 public:
  CSceneNode();

  /// \returns the corresponding cocos2d scene
  cocos2d::Scene* GetScene();
  
  /// \brief initialize the scene of the game
  cocos2d::Scene* CreateScene();

  virtual bool init();

  // TMP, TODO : remove
  void menuCloseCallback(Ref* pSender);

  CREATE_FUNC(CSceneNode);
};

} // namespace LM

#endif /* _CSCENE_H_ */
