#ifndef _CGRIDNODE_H_
#define _CGRIDNODE_H_

#include "CGroupNode.h"

namespace LM
{

class CGridNode : public CGroupNode
{
 private:
	 int m_iRows;
	 int m_iCols;
	
	 int m_iRowIndex;
	 int m_iColIndex;

 public:
  CGridNode(int a_iRows, int a_iCols, EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition);
  virtual void Init() override;

 private:
  virtual cocos2d::Size GetVisibleSize() override;
  virtual cocos2d::Vec2 GetOrigin() override;
  
};


} // namespace LM

#endif /* _CGRIDNODE_H_ */
