#include "../Include/CNode.h"

#include <QDebug>

namespace LM
{

CNode::CNode() : m_iCurrentNode(0), m_pParent(nullptr)
{
}

CNode::~CNode()
{
    for (CNode* itChild : *this)
    {
        delete itChild;
    };
}



void CNode::AddChildNode(CNode* a_pChild)
{
    a_pChild->m_pParent = this;
    m_vChildren.push_back(a_pChild);
}

void CNode::AddChildNodeAtBegin(CNode *a_pChild)
{
    a_pChild->m_pParent = this;
    m_vChildren.insert(m_vChildren.begin(), a_pChild);
}

void CNode::AddChildNodeAt(CNode* a_pChild, const std::string& a_rID)
{
    for(CNode* currentNode : this->m_vChildren)
    {
        // function should be called only on the behaviour tree
        // There should be only CSceneNode in children vector
        if(currentNode->hasID(a_rID))
        {
            int pos = find(m_vChildren.begin(), m_vChildren.end(), currentNode) - m_vChildren.begin();
            qDebug()<<"Found children at index :"<<pos;
            a_pChild->m_pParent = this;
            m_vChildren.insert(m_vChildren.begin() + pos + 1, a_pChild);
            return;
        }
    }
    qDebug("No children found");
}

void CNode::DeleteChildByID(const std::string& a_sID)
{
    int index = 0;
    CNode* tempNode;
    for(CNode* currentNode : this->m_vChildren)
    {
        if(currentNode->hasID(a_sID))
        {
            index = find(m_vChildren.begin(), m_vChildren.end(), currentNode) - m_vChildren.begin();
            tempNode = currentNode;
        }
    }
    this->m_vChildren.erase(m_vChildren.begin() + index);
    delete(tempNode);
}

CNode* CNode::FindChildByID(const std::string& a_sID, bool a_bRecSearch)
{
    std::list<CNode*> frontier(1, this);
    while (!frontier.empty()) {
        for(CNode* currentNode : frontier.front()->m_vChildren)
        {
            if(currentNode->hasID(a_sID))
            {
                return currentNode;
            }
            if (a_bRecSearch)
            {
                frontier.push_back(currentNode);
            }
        }
        frontier.pop_front();
    }
    return nullptr;
    //    for(CNode* currentNode : this->m_vChildren)
    //    {
    //        if(currentNode->hasID(a_sID))
    //        {
    //            return currentNode;
    //        }
    //    }
    //    return nullptr;
}


CNode::Iterator CNode::begin()
{
    return m_vChildren.begin();
}

CNode::Iterator CNode::end()
{
    return m_vChildren.end();
}


CNode* CNode::operator[](const int i)
{
    return m_vChildren[i];
}

/*void CNode::DoForEachChildNode(void(*a_fpCallback)(CNode* a_pChild))
{
    std::for_each(begin(), end(), a_fpCallback);
}*/


void CNode::Finish()
{
    m_pParent->Finish();
}

CNode* CNode::GetCurrentNode()
{
    if (m_iCurrentNode >= 0 && m_iCurrentNode < m_vChildren.size())
        return m_vChildren[m_iCurrentNode];

    return nullptr;
}

bool CNode::OffsetCurrentNode(bool a_bNext)
{
    int i = a_bNext ? 1 : -1;
    if (m_iCurrentNode + i < m_vChildren.size())
    {
        m_iCurrentNode += i;
        return true;
    }

    return false;
}


bool CNode::SetCurrentNode(CNode* a_pNode)
{
    for (int i = 0; i < m_vChildren.size(); ++i)
    {
        if (a_pNode == m_vChildren[i])
        {
            m_iCurrentNode = i;
            return true;
        }
    }
    return false;
}

bool CNode::hasID(const std::string &a_rID)
{
    return false;
}

CNode* CNode::GetOffsetNode(bool a_bNext)
{
    int i = a_bNext ? 1 : -1;
    if (m_iCurrentNode + i < m_vChildren.size() && m_iCurrentNode + i >= 0)
    {
        return m_vChildren[m_iCurrentNode + i];
    }
    return nullptr;
}

std::vector<CNode*> CNode::GetChildren()
{
    return m_vChildren;
}

void CNode::SetChildren(const std::vector<CNode*> &a_vChildren)
{
    m_vChildren = a_vChildren;
}

CNode* CNode::GetParent()
{
    return m_pParent;
}

void CNode::SetParent(CNode *a_pParent)
{
    m_pParent = a_pParent;
}

void CNode::Init()
{
    for (CNode* pChildNode : m_vChildren)
    {
        pChildNode->Init();
    }
}

void CNode::UnInit(bool removeChild)
{
    for (CNode* pChildNode : m_vChildren)
    {
        pChildNode->UnInit(removeChild);
    }
}

void CNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
    for (CNode* pChildNode : m_vChildren)
    {
        pChildNode->ToJson(a_rParent, a_rAllocator);
    }
}

std::string CNode::GetSceneID()
{
    return "";
}

bool CNode::UseFile(const std::string &a_sFilename)
{
    return false;
}

} // namespace LM
