#ifndef _CVIDEONODE_H_
#define _CVIDEONODE_H_

#include <string>
#include "CEntityNode.h"

namespace LM
{

class CVideoNode : public CEntityNode
{
 protected:
  std::string m_sVideoFilename;

 public:
  CVideoNode(const std::string& a_rFilename,
             EAnchor a_eAnchor = EAnchor::CENTER,
             int a_iWidth = 0,
             int a_iHeight = 0,
             int a_iXPosition = 0,
             int a_iYPosition = 0);

  virtual void Init();
  

  virtual void ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator);

};

} // namespace LM

#endif /* _CVIDEONODE_H_ */
