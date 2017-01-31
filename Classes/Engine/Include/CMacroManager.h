#include <string>
#include <iostream>
#include <map>

#include "CJsonParser.h"

namespace LM
{

	class CMacroManager
	{
	public:
		static CMacroManager* Instance();

		std::string CheckDefinition(const std::string& a_sFileName);
		void AddDefinition(const std::string& a_sMacroName, const std::string& a_sMacroDefinition);
		void RemoveDefinition(const std::string& a_sMacroName);
		std::string GetDefinition(const std::string& a_sMacroName);
		bool HasDefinition(const std::string& a_sMacroName);
		void ParseJSON(RefJsonNode a_rJNode, const std::string& a_sBasePath);

	private:
		CMacroManager();
		static CMacroManager* s_pInstance;

		std::map<std::string, std::string> m_mMacros;
	};
}
