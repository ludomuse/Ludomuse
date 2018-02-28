#ifdef LUDOMUSE_EDITOR

#include "../Include/CInfoNode.h"
#include <QDebug>
#include <CProjectManager.h>

namespace LM
{

CInfoNode::CInfoNode(const std::string &a_rFilename, EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition) :
    CSpriteNode(a_rFilename,
                a_eAnchor,
                a_iWidth,
                a_iHeight,
                a_iXPosition,
                a_iYPosition)
{
}

void CInfoNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
{
//    qDebug("Parsing CInfoNode");
    rapidjson::Value info(rapidjson::kObjectType);
    info.AddMember("type", "Info", a_rAllocator);
    if(!m_sID.empty())
    {
        info.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);

    std::string temp = m_sSpriteFilename;
    std::string projectPath = CProjectManager::Instance()->GetProjectPath();
    int index = temp.find(projectPath);
    if(index != std::string::npos)
    {
        temp.erase(index, projectPath.length());
    }
    else
    {
        std::string templatePath = CProjectManager::Instance()->GetInstallPath() + "/templates/";
        int index2 = temp.find(templatePath);
        if(index2 != std::string::npos)
        {
            temp.erase(index2, templatePath.length());
        }
    }
    std::string* string = CProjectManager::Instance()->PushBackSource(temp);
    params.AddMember("source", rapidjson::Value(string->c_str(), string->length()) , a_rAllocator);
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);

    rapidjson::Value children(rapidjson::kArrayType);

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

    info.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(info, a_rAllocator);
}

} // end namespace LM



#endif // LUDOMUSE_EDITOR
