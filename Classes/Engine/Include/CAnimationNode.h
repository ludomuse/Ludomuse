#ifndef _CANIMATIONNODE_H_
#define _CANIMATIONNODE_H_

#include "CEntityNode.h"

namespace LM
{


class CAnimationNode : public CEntityNode
{
 private:
  std::string m_sSpriteSheet;
  std::string m_sFrameFile;
  cocos2d::Animation* m_pAnimation;
  cocos2d::SpriteBatchNode* m_pSpriteBatchNode;

 public:
  CAnimationNode(const std::string& a_rSpriteSheet,
                 const std::string& a_rAFrameFile,
                 EAnchor a_eAnchor = EAnchor::FLOAT,
                 int a_iWidth = 0,
                 int a_iHeight = 0,
                 int a_iXPosition = 0,
                 int a_iYPosition = 0);

  virtual ~CAnimationNode();

  virtual void Init() override;
#ifdef LUDOMUSE_EDITOR
  virtual void ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator);
  #endif

};


} // namespace LM

#endif /* _CANIMATIONNODE_H_ */
