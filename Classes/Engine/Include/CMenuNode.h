#ifndef _CMENUNODE_H_
#define _CMENUNODE_H_

#include "cocos2d.h"

#include "CEntityNode.h"
#include "CCallback.h"

namespace LM
{

// TODO
//typedef void (*FPMenuClickedCallback)(cocos2d::Ref* pSender);

class CMenuNode : public CEntityNode
{
 protected:
  std::string m_sNormalImage;
  std::string m_sSelectedImage;

  cocos2d::Size m_oSize;
  cocos2d::Vec2 m_oOrigin;

  CCallback m_fpClickedCallback;
  
  cocos2d::MenuItemImage* m_pMenuItemImage;

 public:
  CMenuNode(const std::string& a_rNormalImage,
            const std::string& a_rSelectedImage,
            CCallback a_fpCallback,
	        EAnchor a_eAnchor = EAnchor::FLOAT,
			int a_iWidth = 0,
			int a_iHeight = 0,
            int a_iXPosition = 0,
            int a_iYPosition = 0);
  
  virtual void Init() override;
  
  virtual cocos2d::Size GetVisibleSize() override;

  virtual cocos2d::Vec2 GetOrigin() override;

};

} // namespace LM

#endif /* _CMENUNODE_H_ */
