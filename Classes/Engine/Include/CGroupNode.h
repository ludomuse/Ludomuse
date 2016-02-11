#ifndef _CGROUPNODE_H_
#define _CGROUPNODE_H_

#include "CEntityNode.h"

#include "cocos2d.h"

namespace LM
{

class CGroupNode : public CEntityNode
{

private:
	std::string m_sBackgroundSource;

 public:
  CGroupNode(EAnchor a_eAnchor,
                    int a_iWidth,
                    int a_iHeight,
                    int a_iXPosition = 0,
                    int a_iYPosition = 0,
	  const std::string& a_rBackgroundSource = "");
  virtual void Init() override;

  
};


} // namespace LM

#endif /* _CGROUPNODE_H_ */
