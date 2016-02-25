#include "../Include/CPeerNode.h"
#include "../Include/CLabelNode.h"

#include "cocos2d.h"

namespace LM
{

CPeerNode::CPeerNode(CKernel* a_pKernel,
                     const rapidjson::Value& a_rJsonNode,
                     EAnchor a_eAnchor,
                     int a_iWidth,
                     int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CGridNode(2, 3, a_eAnchor, a_iHeight, a_iWidth, a_iXPosition, a_iYPosition),
    m_pKernel(a_pKernel),
    m_rChildrenStyle(a_rJsonNode)
{
  
}


void CPeerNode::Init()
{
  CGridNode::Init();
  m_pKernel->GetPeers();
}


void CPeerNode::AddPeers(const std::vector<std::string>& a_vPeers)
{
  // UnInit all children from scene
  UnInit();
  // delete nodes from memory
  for (CNode* itNode : m_vChildren)
  {
    delete itNode;
  }

  m_vChildren.clear();

  int i = 0;
  // add new children
  for (const std::string& sPeer : a_vPeers)
  {
    // parse the json to get the style of the child
    // create the cild
    m_pKernel->GetJsonParser()->ParseJson(m_rChildrenStyle, this);
    

    // go down in the child find a Label and replace its text with sPeer
    ReplaceLabelValue(m_vChildren[i], sPeer);

  }

  // finally initialize entities in the scene
  CGridNode::Init();
}


bool CPeerNode::ReplaceLabelValue(CNode* a_pNode, const std::string& a_sPeerName)
{
  CLabelNode* pLabelNode = dynamic_cast<CLabelNode*>(a_pNode);
  if (pLabelNode)
  {
	  cocos2d::Label* pLabel = dynamic_cast<cocos2d::Label*>(pLabelNode->GetCocosEntity());
	  if (pLabel)
	  {
		  pLabel->setString(a_sPeerName);
	  }
    return true;
  }
  else
  {
    for (CNode* itNode : *a_pNode)
    {
      if (! ReplaceLabelValue(itNode, a_sPeerName))
        break;
    }
  }
  return false;
}


} // namespace LM
