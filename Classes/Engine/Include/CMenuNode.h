#ifndef _CMENUNODE_H_
#define _CMENUNODE_H_

#include "cocos2d.h"

#include "CEntityNode.h"


namespace LM
{

typedef void (*FPMenuClickedCallback)(cocos2d::Ref* pSender);

class CMenuNode : public CEntityNode
{
 protected:
  std::string m_sNormalImage;
  std::string m_sSelectedImage;

  FPMenuClickedCallback m_fpClickedCallback;
  
  cocos2d::MenuItemImage* m_pMenuItemImage;

 public:
  CMenuNode(const std::string& a_rNormalImage,
            const std::string& a_rSelectedImage,
            FPMenuClickedCallback a_fpCallback,
	        EAnchor a_eAnchor = EAnchor::FLOAT,
            int a_iXPosition = 0,
            int a_iYPosition = 0);
  
  virtual void Init();
  
};

} // namespace LM

#endif /* _CMENUNODE_H_ */
