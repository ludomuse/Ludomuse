#ifndef _CSPRITENODE_H_
#define _CSPRITENODE_H_

#include <string>
#include "cocos2d.h"

#include "CEntityNode.h"

namespace LM
{

class CSpriteNode : public CEntityNode
{
 protected:
  /// \brief the system filename of the sprite
  std::string m_sSpriteFilename;
  /// \brief The cocos2d sprite
  cocos2d::Sprite* m_pSprite;

 public:
  CSpriteNode(const std::string& a_rFilename, int a_iXPosition, int a_iYPosition);

  virtual void Init();

  cocos2d::Sprite* operator() ();
};

} // namespace LM


#endif /* _CSPRITENODE_H_ */
