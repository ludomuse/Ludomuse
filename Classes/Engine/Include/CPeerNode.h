#ifndef _CPEERNODE_H_
#define _CPEERNODE_H_


#include "CGridNode.h"

#include "../../cocos2d/external/json/document.h"


#include <vector>

namespace LM
{

class CPeerNode : public CGridNode
{
 private:
  CKernel* m_pKernel;
  const rapidjson::Value& m_rChildrenStyle;

 public:
  CPeerNode(CKernel* a_pKernel, const rapidjson::Value& a_rJsonNode, EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition);

  virtual void Init() override;

  virtual void ClearChildren();

  virtual void AddPeers(const std::vector<std::string>& a_vPeers);

  virtual void ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator);


 private: // methods
  bool ReplaceLabelValue(CNode* a_pNode, const std::string& a_sPeerName);
};


} // namespace LM

#endif /* _CPEERNODE_H_ */
