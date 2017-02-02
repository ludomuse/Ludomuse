#include "../Include/CScratchNode.h"
#include "../Include/CMacroManager.h"
#include "CProjectManager.h"

using namespace cocos2d;

namespace LM
{

  CScratchNode::CScratchNode(const std::string& a_rSpriteFilename,
                 EAnchor a_eAnchor,
                 int a_iWidth,
                 int a_iHeight,
                 int a_iXPosition,
                 int a_iYPosition) :
    CSpriteNode(a_rSpriteFilename,
        a_eAnchor,
        a_iWidth,
        a_iHeight,
        a_iXPosition,
        a_iYPosition)
  {
  }


  void CScratchNode::Init()
  {
      CSpriteNode::Init();
  }


  void CScratchNode::DrawTouch(const Vec2& a_oTouchLocation)
  {

  }

  void CScratchNode::ToJson(rapidjson::Value& a_rParent, rapidjson::Document::AllocatorType& a_rAllocator)
  {
      // TODO
      rapidjson::Value spriteNode(rapidjson::kObjectType);
      spriteNode.AddMember("type", "Scratch", a_rAllocator);
      if(!m_sID.empty())
      {
          spriteNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
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

      spriteNode.AddMember("params", params, a_rAllocator);
      a_rParent.PushBack(spriteNode, a_rAllocator);
      /*{
              "type": "Image",
              "params": {
                "source": "cache/white.png",
                "anchor": 8,
                "width": 100,
                "height": 100,
                "x": 0,
                "y": 0
              }
            },
            */
  }

} // namespace LM
