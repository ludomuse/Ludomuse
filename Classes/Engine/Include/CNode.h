#ifndef _CNODE_H_
#define _CNODE_H_

#include <vector>
#include "cocos2d.h"


namespace LM
{

class CNode
{
 protected:
  std::vector<CNode*> m_vChildren;
  CNode* m_pCurrentNode;

 public:
  CNode();
  virtual ~CNode();
  virtual bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) = 0;
  void AddChild(CNode* a_oChild);
};


} // namespace LM

#endif /* _CNODE_H_ */
