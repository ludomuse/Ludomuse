#include "../Include/CStats.h"

namespace LM
{

//////////////////////// SScreenStats functions

SScreenStats::SScreenStats()
{
  reinit();
}

void SScreenStats::reinit()
{
  time = 0;

  nbInteractions = 0;

  nbTouches = 0;
  nbMoves = 0;

  nbValidTouches = 0;

  nbValidDrops = 0;
  nbInvalidDrops = 0;

  nbValidAnswers = 0;
  nbInvalidAnswers = 0;
}


std::ostream& operator<<(std::ostream& os, const SScreenStats& s)
{
	os << "[LUDO_STATS] time : " << s.time << std::endl;
	os << "[LUDO_STATS] nbInteractions : " << s.nbInteractions << std::endl;
	os << "[LUDO_STATS] nbTouches : " << s.nbTouches << std::endl;
	os << "[LUDO_STATS] nbMoves : " << s.nbMoves << std::endl;
	os << "[LUDO_STATS] nbValidTouches : " << s.nbValidTouches << std::endl;
	os << "[LUDO_STATS] nbValidDrops : " << s.nbValidDrops << std::endl;
	os << "[LUDO_STATS] nbInvalidDrops : " << s.nbInvalidDrops << std::endl;
	os << "[LUDO_STATS] nbValidAnswers : " << s.nbValidAnswers << std::endl;
	os << "[LUDO_STATS] nbInvalidAnswers : " << s.nbInvalidAnswers << std::endl;
	return os;
}

///////////////////////// CStats functions
#pragma region SINGLETON_FUNCTIONS
CStats::CStats()
{

}

CStats::CStats(const CStats&)
{

}

CStats& CStats::operator=(const CStats&)
{
	return *this;
}

CStats* CStats::Instance()
{
  static CStats instance;

  return &instance;
}
#pragma endregion SINGLETON_FUNCTIONS




void CStats::PushStats(const std::string& a_rScreenID)
{
	milliseconds oScreenDuration = duration_cast<milliseconds>(system_clock::now() - m_oScreenStartTime);
  m_oCurrentScreenStats.time = oScreenDuration.count();
  
  m_mScreensStats.insert(std::pair<std::string, SScreenStats>(a_rScreenID, m_oCurrentScreenStats));

  std::stringstream ss;
  ss << m_oCurrentScreenStats;

  CCLOG("pushing stats");
  CCLOG("%s", ss.str().c_str());

  m_oCurrentScreenStats.reinit();

}

void CStats::StartStats()
{
  m_oScreenStartTime = system_clock::now();
}


std::map<std::string, SScreenStats> CStats::GetStats()
{
	return m_mScreensStats;
}



void CSerializableStats::writeOn(bytes* msg)
{
	int iSize = m_mScreensStats.size();
	*msg << iSize;
	std::map<std::string, SScreenStats>::iterator itScreen;
	for (itScreen = m_mScreensStats.begin(); itScreen != m_mScreensStats.end(); ++itScreen)
	{
		const SScreenStats& stats = itScreen->second;
		*msg << itScreen->first;
		*msg << stats.time;
		*msg << stats.nbInteractions;
		*msg << stats.nbTouches;
		*msg << stats.nbValidTouches;
		*msg << stats.nbMoves;
		*msg << stats.nbValidDrops;
		*msg << stats.nbInvalidDrops;
		*msg << stats.nbValidAnswers;
		*msg << stats.nbInvalidAnswers;
	}
}

void CSerializableStats::readOn(bytes* msg)
{
	int iSize;
	*msg >> iSize;

	std::map<std::string, SScreenStats> mScreenStats;

	for (int i = 0; i < iSize; ++i)
	{
		SScreenStats stats;
		std::string screenID;

		*msg >> screenID;
		*msg >> stats.time;
		*msg >> stats.nbInteractions;
		*msg >> stats.nbTouches;
		*msg >> stats.nbValidTouches;
		*msg >> stats.nbMoves;
		*msg >> stats.nbValidDrops;
		*msg >> stats.nbInvalidDrops;
		*msg >> stats.nbValidAnswers;
		*msg >> stats.nbInvalidAnswers;

		mScreenStats.insert(std::pair<std::string, SScreenStats>(screenID, stats));
	}

}


} // namespace LM
