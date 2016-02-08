#ifndef _CGRIDNODE_H_
#define _CGRIDNODE_H_

#include "CEntityNode.h"

namespace LM
{

class CGridNode : public CEntityNode
{
 private:
  

 public:
  CGridNode(EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition);
  virtual void Init();


 private:
  virtual cocos2d::Size GetParentVisibleSize();
  virtual cocos2d::Vec2 GetParentOrigin();
  
};


} // namespace LM

#endif /* _CGRIDNODE_H_ */
