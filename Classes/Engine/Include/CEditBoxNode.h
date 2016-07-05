#ifndef _CEDITBOXNODE_H_
#define _CEDITBOXNODE_H_

#include "CEntityNode.h"


namespace LM
{

class CEditBoxNode : public CEntityNode
{
 public:
  CEditBoxNode(EAnchor a_eAnchor = EAnchor::CENTER, int a_iWidth = 100, int a_iHeight = 0, int a_iXPosition = 0, int a_iYPosition = 0);
  virtual void Init() override;
  virtual void ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator);
};

} // namespace LM

#endif /* _CEDITBOXNODE_H_ */
