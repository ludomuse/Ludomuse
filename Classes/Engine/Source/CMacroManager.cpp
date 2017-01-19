#include "Classes\Engine\Include\CMacroManager.h"

#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"

namespace LM
{

	CMacroManager* CMacroManager::s_pInstance = 0;

	CMacroManager::CMacroManager()
	{

	}

	CMacroManager* CMacroManager::Instance()
	{
		if (s_pInstance == 0) {
			s_pInstance = new CMacroManager;
		}
		return s_pInstance;
	}

	void CMacroManager::AddDefinition(const std::string& a_sMacroName, const std::string& a_sMacroDefinition)
	{
		m_mMacros[a_sMacroName] = a_sMacroDefinition;
	}

	void CMacroManager::RemoveDefinition(const std::string& a_sMacroName)
	{
		m_mMacros.erase(a_sMacroName);
	}

	std::string CMacroManager::GetDefinition(const std::string& a_sMacroName)
	{
		try {
			return m_mMacros.at(a_sMacroName);
		}
		catch (std::out_of_range)
		{
			return "";
		}
	}

	bool CMacroManager::HasDefinition(const std::string& a_sMacroName)
	{
		try {
			m_mMacros.at(a_sMacroName);
		}
		catch (std::out_of_range)
		{
			return false;
		}
		return true;
	}

	void CMacroManager::ParseJSON(RefJsonNode a_rJNode, const std::string& a_sBasePath)
	{
		for (rapidjson::Value::ConstMemberIterator itr = a_rJNode.MemberBegin();
			itr != a_rJNode.MemberEnd(); ++itr)
		{
			assert(itr->name.IsString() && itr->value.IsString());
			AddDefinition(itr->name.GetString(), a_sBasePath + itr->value.GetString());
		}
	}

	std::string CMacroManager::CheckDefinition(const std::string& a_sFileName)
	{
		if (a_sFileName.size() > 0 && a_sFileName.at(0) == '#')
		{
			try {
				return m_mMacros.at(a_sFileName);
			}
			catch (std::out_of_range)
			{
				return "";
			}
		}
		return a_sFileName;
	}
}