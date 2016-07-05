#include "../Include/CJsonParser.h"

#include "reader.h"
#include "filestream.h"
#include "stringbuffer.h"

#include "../Include/CValidateSceneVisitor.h"

#include <QDebug>

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
        CEventCallback oCallback("GotoSceneID", m_pKernel, &CKernel::GotoScreenID,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "ValidateScene")
	{
        CEventCallback oCallback("ValidateScene", m_pKernel, &CKernel::ValidateScene,
            SEvent(SEvent::BOOLEAN, a_pEntity, sCallbackString, a_rListener["params"]["arg"].GetBool()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Validate")
	{
        CEventCallback oCallback("Validate", m_pKernel, &CKernel::Validate,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "ConnectPeer")
	{
        CEventCallback oCallback("ConnectPeer", m_pKernel, &CKernel::Connect,
            SEvent(SEvent::NONE, a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Show")
	{
		bool bShow = true;
		if (a_rListener["params"].HasMember("arg"))
			bShow = a_rListener["params"]["arg"].GetBool();

        CEventCallback oCallback("Show", m_pKernel, &CKernel::SetNodeVisible,
            SEvent(SEvent::BOOLEAN, a_pEntity, sCallbackString, bShow));
		a_pEntity->AddListener(sType, oCallback);
		return false;
	}
	else if (sCallbackString == "ShowBack")
	{
        CEventCallback oCallback("ShowBack", m_pKernel, &CKernel::SetNodeVisible,
            SEvent(SEvent::BOOLEAN, a_pEntity, sCallbackString, true));
		a_pEntity->AddListener(sType, oCallback);
		return true;
	}
	else if (sCallbackString == "SetText")
	{
        CEventCallback oCallback("SetText", m_pKernel, &CKernel::SetText,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString(), true));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SetPlayerID")
	{
        CEventCallback oCallback("SetPlayerID", m_pKernel, &CKernel::SetPlayerID,
            SEvent(SEvent::NUMBER, a_pEntity, sCallbackString, true, a_rListener["params"]["arg"].GetInt()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else  if (sCallbackString == "SetPlayerName")
	{
        CEventCallback oCallback("SetPlayerName", m_pKernel, &CKernel::SetPlayerName,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SendMessage")
	{
        CEventCallback oCallback("SendMessage", m_pKernel, &CKernel::SendNetworkMessage,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "LocalMessage")
	{
        CEventCallback oCallback("LocalMessage", m_pKernel, &CKernel::LocalMessage,
            SEvent(SEvent::STRING,a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Fade")
	{
        CEventCallback oCallback("Fade", m_pKernel, &CKernel::FadeEntity,
            SEvent(SEvent::NONE, a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "DisableEvent")
	{
        CEventCallback oCallback("DisableEvent", m_pKernel, &CKernel::DisableEvent,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "EnableEvent")
	{
        CEventCallback oCallback("EnableEvent", m_pKernel, &CKernel::EnableEvent,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "AnchorEntity")
	{
        CEventCallback oCallback("AnchorEntity", m_pKernel, &CKernel::AnchorEntityCallback,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "PlaySound")
	{
        CEventCallback oCallback("PlaySound", m_pKernel, &CKernel::PlaySoundCallback,
            SEvent(SEvent::STRING, a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "RefreshPeers")
	{
        CEventCallback oCallback("RefreshPeers", m_pKernel, &CKernel::RefreshPeers,
            SEvent(SEvent::NONE, a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "EndGame")
	{
        CEventCallback oCallback("EndGame", m_pKernel, &CKernel::EndGame,
            SEvent(SEvent::NONE, a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else
	{
        CEventCallback oCallback(sCallbackString, m_pKernel, nullptr);
        a_pEntity->AddListener(sType, oCallback);
	}

	return true;
}


} // namespace LM
