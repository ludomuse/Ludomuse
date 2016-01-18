#ifndef _CLABELNODE_H_
#define _CLABELNODE_H_

#include <string>
#include "cocos2d.h"

#include "CEntityNode.h"

namespace LM
{

class CLabelNode : public CEntityNode
{
 protected:
  std::string m_sText;
  std::string m_sFontName;
  int m_iFontSize;

  //cocos2d::Label* m_pLabel;

 public:
  CLabelNode(const std::string& a_rText, const std::string& a_rFontName, int a_iFontSize,
             EAnchor a_eAnchor = EAnchor::FLOAT, 
	         int a_iXPosition = 0, 
	         int a_iYPosition = 0);

  /// \brief init this node
  virtual void Init();

};

} // namespace LM

#endif /* _CLABELNODE_H_ */
