#ifndef CINFONODE_H
#define CINFONODE_H

#include "CSpriteNode.h"
namespace LM
{
class CInfoNode : public CSpriteNode
{
public:
    CInfoNode(const std::string& a_rFilename,
              EAnchor a_eAnchor = EAnchor::FLOAT,
              int a_iWidth = 0,
              int a_iHieght = 0,
              int a_iXPosition = 0,
              int a_iYPosition = 0);

    virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);
};
}
#endif // CINFONODE_H
