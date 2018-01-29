#include "../Include/CPeerNode.h"
#include "../Include/CLabelNode.h"

#include "../Include/CJsonParser.h"

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
    CGridNode(4, 4, a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_pKernel(a_pKernel),
    m_rChildrenStyle(a_rJsonNode)
{
  
}


void CPeerNode::Init()
{
	ClearChildren();

	CGridNode::Init();
	m_pKernel->GetPeers();
}


void CPeerNode::ClearChildren()
{
	for (CNode* itNode : m_vChildren)
	{
		delete itNode;
	}
	m_vChildren.clear();
}


void CPeerNode::AddPeers(const std::vector<std::string>& a_vPeers)
{
  // UnInit all children from scene
	for (CNode* itNode : m_vChildren)
	{
		itNode->UnInit();
	}
  // delete nodes from memory
  ClearChildren();

  // add new children
  for (const std::string& sPeer : a_vPeers)
  {
    // parse the json to get the style of the child
    // create the child
    m_pKernel->GetJsonParser()->ParseJson(m_rChildrenStyle, this);
    

    // go down in the child find a Label and replace its text with sPeer
  }

  // finally initialize entities in the scene
  CGridNode::Init();

  for (int i = 0; i < a_vPeers.size(); ++i)
  {
	  ReplaceLabelValue(m_vChildren[i], a_vPeers[i]);
  }
}


bool CPeerNode::ReplaceLabelValue(CNode* a_pNode, const std::string& a_sPeerName)
{
  CLabelNode* pLabelNode = dynamic_cast<CLabelNode*>(a_pNode);
  if (pLabelNode)
  {
	  pLabelNode->SetText(a_sPeerName);
    return true;
  }
  else
  {
    for (CNode* itNode : *a_pNode)
    {
      if (ReplaceLabelValue(itNode, a_sPeerName))
        break;
    }
  }
  return false;
}


} // namespace LM
