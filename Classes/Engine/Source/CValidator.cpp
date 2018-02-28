#include "../Include/CValidator.h"
#include "../Include/CCallback.h"
#include "../Include/CSoundManager.h"

#include "../../Modules/Util/Include/CStats.h"

namespace LM
{

CValidator::CValidator(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
}


void CValidator::SetSound(const std::string& a_rSound)
{
	m_sSound = a_rSound;
}

void CValidator::AddID(const std::string& a_sID)
{
  m_oIDs.insert(a_sID);
}


void CValidator::Validate(const std::string& a_sID)
{
  int iOldSize = m_oIDs.size();
  std::set<std::string>::iterator itID = m_oIDs.find(a_sID);
  if (itID != m_oIDs.end())
  {
    CCLOG("validating id : %s", (*itID).c_str());
    m_oIDs.erase(itID);

    M_STATS_SCREEN.nbValidAnswers++;
  }
  else
  {
    M_STATS_SCREEN.nbInvalidAnswers++;
  }


  if (m_oIDs.size() == 0 && iOldSize > m_oIDs.size())
  {
    CCLOG("Validate scene");
	m_pKernel->ValidateScene(SEvent(), nullptr);
        // TODO : quick hack, fix linux and maybe windows stack for better solution
	m_pKernel->SendNetworkMessage("kernel:Validate");

	if (m_sSound != "")
		m_pKernel->m_pSoundManager->PlaySound(m_sSound);
  }
}
#ifdef LUDOMUSE_EDITOR
void CValidator::ToJson(rapidjson::Value &parent, rapidjson::Document::AllocatorType &allocator)
{
    rapidjson::Value validator(rapidjson::kObjectType);
    validator.AddMember("type", "Validator", allocator);
    rapidjson::Value params(rapidjson::kObjectType);
    rapidjson::Value ids(rapidjson::kArrayType);
    for(const std::string& currentString : m_oIDs)
    {
        ids.PushBack(rapidjson::Value(currentString.c_str(), currentString.length()), allocator);
    }
    params.AddMember("ids", ids, allocator);
    params.AddMember("sound", rapidjson::Value(m_sSound.c_str(), m_sSound.length()), allocator);
    validator.AddMember("params", params, allocator);
    parent.PushBack(validator, allocator);
//    {
//                "type": "Validator",
//                "params": {
//                  "ids": [
//                    "local",
//                    "distant"
//                  ],
//                  "sound": "cache/audio/gong.mp3"
//                }
//              }

}
#endif
}
