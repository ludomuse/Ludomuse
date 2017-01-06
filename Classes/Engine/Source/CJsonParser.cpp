#include "../Include/CJsonParser.h"

#include "../../cocos2d/external/json/reader.h"
#include "../../cocos2d/external/json/filestream.h"
#include "../../cocos2d/external/json/stringbuffer.h"

#include "../Include/CValidateSceneVisitor.h"
#include "Classes\Engine\Include\CMacroManager.h"

using namespace cocos2d;

namespace LM
{

CJsonParser::CJsonParser(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{

}


std::string CJsonParser::GetBasePath()
{
	return m_sBasePath;
}

std::string CJsonParser::NormalizePath(const std::string& a_sFileName)
{
	if (a_sFileName.size() > 0 && a_sFileName.at(0) == '#')
	{
		return a_sFileName;
	}
	else
	{
		return (m_sBasePath + a_sFileName);
	}
}

void CJsonParser::BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename)
{

	// init json document

	std::string sJsonString = cocos2d::FileUtils::getInstance()->getStringFromFile(a_sFilename);

	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(a_sFilename);
	int lastSlash = fullPath.find_last_of("/");
	while (lastSlash == fullPath.size() - 1)
	{
		fullPath = fullPath.substr(0, lastSlash);
		lastSlash = fullPath.find_last_of("/");
	}
	fullPath = fullPath.substr(0, lastSlash + 1); // +1 to save the last slash
	m_sBasePath = fullPath;
	m_oDocument.Parse(sJsonString.c_str());

		if (m_oDocument.HasMember("app"))
	{
		//ParseJsonRoot(m_oDocument["app"], a_pRoot);
		if (m_oDocument["app"].HasMember("debug"))
		{
			m_pKernel->m_bDebugMode = m_oDocument["app"]["debug"].GetBool();
			Director::getInstance()->setDisplayStats(true);
		}

		if (m_oDocument["app"].HasMember("macros"))
		{
			RefJsonNode rMacros = m_oDocument["app"]["macros"];
			CMacroManager::Instance()->ParseJSON(rMacros, m_sBasePath);
		}

		assert(m_oDocument["app"].HasMember("scenes"));
		RefJsonNode rScenesID = m_oDocument["app"]["scenes"];
		for (int i = 0; i < rScenesID.Size(); ++i)
		{
			for (int j = 0; j < rScenesID[i].Size(); ++j)
			{
				m_pKernel->AddSceneID(i, rScenesID[i][j].GetString());
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
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "ValidateScene")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::ValidateScene,
			SEvent(a_pEntity, sCallbackString, a_rListener["params"]["arg"].GetBool()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Validate")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::Validate,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "ConnectPeer")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::Connect,
			SEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Show")
	{
		bool bShow = true;
		if (a_rListener["params"].HasMember("arg"))
			bShow = a_rListener["params"]["arg"].GetBool();

		CEventCallback oCallback(m_pKernel, &CKernel::SetNodeVisible,
			SEvent(a_pEntity, sCallbackString, bShow));
		a_pEntity->AddListener(sType, oCallback);
		return false;
	}
	else if (sCallbackString == "ShowBack")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetNodeVisible,
			SEvent(a_pEntity, sCallbackString, true));
		a_pEntity->AddListener(sType, oCallback);
		return true;
	}
	else if (sCallbackString == "Colorize")
	{
		bool bColor = true;
		if (a_rListener["params"].HasMember("arg"))
			bColor = a_rListener["params"]["arg"].GetBool();

		CEventCallback oCallback(m_pKernel, &CKernel::SetNodeColored,
			SEvent(a_pEntity, sCallbackString, bColor));
		a_pEntity->AddListener(sType, oCallback);
		a_pEntity->Colorize(false);
		return true;
	}
	else if (sCallbackString == "SetText")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetText,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString(), true));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SetPlayerID")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetPlayerID,
			SEvent(a_pEntity, sCallbackString, true, a_rListener["params"]["arg"].GetInt()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else  if (sCallbackString == "SetPlayerName")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SetPlayerName,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "SendMessage")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::SendNetworkMessage,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "LocalMessage")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::LocalMessage,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "Fade")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::FadeEntity,
			SEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "DisableEvent")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::DisableEvent,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "EnableEvent")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::EnableEvent,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "AnchorEntity")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::AnchorEntityCallback,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "PlaySound")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::PlaySoundCallback,
			SEvent(a_pEntity, a_rListener["params"]["arg"].GetString()));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "RefreshPeers")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::RefreshPeers,
			SEvent(a_pEntity));
		a_pEntity->AddListener(sType, oCallback);
	}
	else if (sCallbackString == "EndGame")
	{
		CEventCallback oCallback(m_pKernel, &CKernel::EndGame,
			SEvent(a_pEntity));
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
