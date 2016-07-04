#include "CInfoNode.h"
#include <QDebug>

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
    // To do
    qDebug("Parsing CInfoNode");
    rapidjson::Value info(rapidjson::kObjectType);
    info.AddMember("type", "Info", a_rAllocator);

    a_rParent.PushBack(info, a_rAllocator);
}
}
