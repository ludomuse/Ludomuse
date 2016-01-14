#include "../Include/CJsonParser.h"

#include "../../cocos2d/external/json/reader.h"
#include "../../cocos2d/external/json/filestream.h"
#include "../../cocos2d/external/json/stringbuffer.h"


using namespace cocos2d;

namespace LM
{

void CJsonParser::BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename)
{

  // init json document
  std::string sJsonString = cocos2d::FileUtils::getInstance()->getStringFromFile(a_sFilename);

  // TODO delete <0> if useless
  m_oDocument.Parse<0>(sJsonString.c_str());

  if (m_oDocument.HasMember("app"))
  {
	  ParseJson<0>(m_oDocument["app"], a_pRoot);
  }
}



} // namespace LM
