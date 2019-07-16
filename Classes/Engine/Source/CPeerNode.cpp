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
    CCLOG("CPeerNode::Init");

    ClearChildren();
    CGridNode::Init();

#ifndef LUDOMUSE_EDITOR
    m_pKernel->GetPeers();
#else
    if (m_vChildren.size() == 0) {
      m_pKernel->GetJsonParser()->ParseJson(m_rChildrenStyle, this);
    }
    ReplaceLabelValue(m_vChildren[0], "Pair");
#endif
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
    CCLOG("ADD PEERS called m_vChildren : %d, a_vPeers : %d", m_vChildren.size(), a_vPeers.size());

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
#ifdef LUDOMUSE_EDITOR
void CPeerNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{

    rapidjson::Value peerNode(rapidjson::kObjectType);
    peerNode.AddMember("type", "Peers", a_rAllocator);
    if(!m_sID.empty())
    {
        peerNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }
    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

    if(!this->m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if(!this->m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    peerNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(peerNode, a_rAllocator);
}
#endif

} // namespace LM
