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
	  if (m_oDocument["app"].HasMember("debug"))
	  {
		  m_pKernel->m_bDebugMode = m_oDocument["app"]["debug"].GetBool();
		  Director::getInstance()->setDisplayStats(true);
	  }

	  assert(m_oDocument["app"].HasMember("scenes"));
	  RefJsonNode rScenesID = m_oDocument["app"]["scenes"];
	  for (int i = 0; i < rScenesID.Size(); ++i)
	  {
		  for (int j = 0; j < rScenesID[i].Size(); ++j)
		  {
			  m_pKernel->AddSceneID(j, rScenesID[i][j].GetString());
		  }
	  }

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

	std::string sCallbackString = "";

	if (a_rListener["params"].HasMember("callback"));
		sCallbackString = a_rListener["params"]["callback"].GetString();

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
	else if (sCallbackString == "Validate")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::Validate,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "ConnectPeer")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::Connect,
			CEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Show")
	{
		bool bShow = true;
		if (a_rListener["params"].HasMember("arg"))
			bShow = a_rListener["params"]["arg"].GetBool();

		CEventCallback oCallback(m_pKernel, &CKernel::SetNodeVisible,
			CEvent(a_pEntity, sCallbackString, bShow));
		a_pEntity->AddListener(sType, oCallback);
		return false;
	}
	else if (sCallbackString == "ShowBack")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetNodeVisible,
			CEvent(a_pEntity, sCallbackString, true));
		a_pEntity->AddListener(sType, oCallback);
		return true;
	}
	else if (sCallbackString == "SetText")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetText,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString(), true));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SetPlayerID")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetPlayerID,
			CEvent(a_pEntity, sCallbackString, true, a_rListener["params"]["arg"].GetInt()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else  if (sCallbackString == "SetPlayerName")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetPlayerName,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SendMessage")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SendNetworkMessage,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "LocalMessage")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::LocalMessage,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Fade")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::FadeEntity,
			CEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "DisableEvent")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::DisableEvent,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "EnableEvent")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::EnableEvent,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "AnchorEntity")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::AnchorEntityCallback,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "PlaySound")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::PlaySoundCallback,
			CEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "RefreshPeers")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::RefreshPeers,
			CEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else
	{
		CEventCallback oCallback(m_pKernel, nullptr);
		a_pEntity->AddListener(sType, oCallback);
	}

	return true;
}


} // namespace LM
