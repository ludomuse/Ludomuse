#ifndef _CEDITBOXNODE_H_
#define _CEDITBOXNODE_H_

#include "CEntityNode.h"


namespace LM
{

class CEditBoxNode : public CEntityNode
{
 public:
  CEditBoxNode(EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition);
  virtual void Init() override;
};

} // namespace LM

#endif /* _CEDITBOXNODE_H_ */
