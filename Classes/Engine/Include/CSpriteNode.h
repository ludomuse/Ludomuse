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
  //cocos2d::Sprite* m_pSprite;

 public:
	 CSpriteNode(const std::string& a_rFilename,
			  EAnchor a_eAnchor = EAnchor::FLOAT,
			  int a_iWidth = 0,
			  int a_iHieght = 0,
	          int a_iXPosition = 0, 
	          int a_iYPosition = 0);

  virtual void Init();

};

} // namespace LM


#endif /* _CSPRITENODE_H_ */
