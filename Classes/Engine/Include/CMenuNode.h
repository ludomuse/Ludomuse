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

  CCallback<CKernel, cocos2d::Ref*> m_fpClickedCallback;
  
  cocos2d::MenuItemImage* m_pMenuItemImage;

  std::string m_sAction;

 public:
  CMenuNode(const std::string& a_rNormalImage,
            const std::string& a_rSelectedImage,
            CCallback<CKernel, cocos2d::Ref*> a_fpCallback,
	        EAnchor a_eAnchor = EAnchor::FLOAT,
			int a_iWidth = 0,
			int a_iHeight = 0,
            int a_iXPosition = 0,
            int a_iYPosition = 0,
            const std::string& a_rAction = "" );
  
  virtual void Init() override;
  
  virtual cocos2d::Size GetVisibleSize() override;

  virtual cocos2d::Vec2 GetOrigin() override;

  virtual cocos2d::Node* GetCocosEntity() override;

  std::string GetAction();

  std::string GetText();

  virtual void Show(bool a_bVisible) override;

  virtual void ToJson(rapidjson::Value &parent, rapidjson::Document::AllocatorType &allocator);

  void emitMenuNodeTouched(cocos2d::Ref* a_pRef, CEntityNode* a_pDummy);

  /// \brief switch action based on argument
  /// \param bool a_bIsNext : true -> action set at next
  /// false -> action set at prev
  void SetNavAction(bool a_bIsNext);

  void SetText(std::string a_sText);

};

} // namespace LM

#endif /* _CMENUNODE_H_ */
