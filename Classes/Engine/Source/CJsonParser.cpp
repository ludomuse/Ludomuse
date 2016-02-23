#include "../Include/CJsonParser.h"

#include "../../cocos2d/external/json/reader.h"
#include "../../cocos2d/external/json/filestream.h"
#include "../../cocos2d/external/json/stringbuffer.h"

#include "../Include/CValidateSceneVisitor.h"

using namespace cocos2d;

namespace LM
{

CJsonParser::CJsonParser(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{

}

void CJsonParser::BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename)
{

  // init json document
  std::string sJsonString = cocos2d::FileUtils::getInstance()->getStringFromFile(a_sFilename);

  m_oDocument.Parse(sJsonString.c_str());

  if (m_oDocument.HasMember("app"))
  {
	  //ParseJsonRoot(m_oDocument["app"], a_pRoot);

	  assert(m_oDocument["app"].HasMember("screens"));
	  RefJsonNode rScenes = m_oDocument["app"]["screens"];
	  for (int i = 0; i < rScenes.Size(); ++i)
	  {
		// recursively parse all the nodes in the json file
		// using the specialised template where needed
	  	ParseJson(rScenes[i], a_pRoot);
	  }
  }
}


bool CJsonParser::ParseCallback(RefJsonNode a_rListener, CEntityNode* a_pEntity)
{

	std::string sType = a_rListener["type"].GetString();
	std::string sCallbackString = a_rListener["params"]["callback"].GetString();

	if (sType == "Touch")
	{
		if (sCallbackString == "GotoSceneID")
		{
			CEventCallback oCallback(m_pKernel, &CKernel::GotoScreenID, 
				CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
			a_pEntity->AddListener(sType, oCallback);
		}
		else if (sCallbackString == "ValidateScene")
		{
			CEventCallback oCallback(m_pKernel, &CKernel::ValidateScene,
				CEvent(a_pEntity, sCallbackString, a_rListener["params"]["arg"].GetBool()));
			a_pEntity->AddListener(sType, oCallback);
		}
	}
	else if (sType == "Move")
	{
		CEventCallback oCallback(m_pKernel, nullptr);
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sType == "Drop")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::ValidateScene,
			CEvent(a_pEntity, sCallbackString, a_rListener["params"]["arg"].GetBool()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sType == "Validate")
	{
		if (sCallbackString == "show")
		{
			CEventCallback oCallback(m_pKernel, &CKernel::SetNodeVisible, 
				CEvent(a_pEntity, sCallbackString, true));
			a_pEntity->AddListener(sType, oCallback);
			return false;
		}
	}
	return true;
}


} // namespace LM
