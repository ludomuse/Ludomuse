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
	os << "time : " << s.time << std::endl;
	os << "nbInteractions : " << s.nbInteractions << std::endl;
	os << "nbTouches : " << s.nbTouches << std::endl;
	os << "nbMoves : " << s.nbMoves << std::endl;
	os << "nbValidTouches : " << s.nbValidTouches << std::endl;
	os << "nbValidDrops : " << s.nbValidDrops << std::endl;
	os << "nbInvalidDrops : " << s.nbInvalidDrops << std::endl;
	os << "nbValideAnswers : " << s.nbValidAnswers << std::endl;
	os << "nbInvalidAnswers : " << s.nbInvalidAnswers << std::endl;
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
  m_oCurrentScreenStats.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  
  m_mScreensStats.insert(std::pair<std::string, SScreenStats>(a_rScreenID, m_oCurrentScreenStats));

  std::stringstream ss;
  ss << m_oCurrentScreenStats;

  CCLOG("pushing stats");
  CCLOG("%s", ss.str().c_str());

  m_oCurrentScreenStats.reinit();

}

void CStats::StartStats()
{
  std::chrono::system_clock::now();

}

} // namespace LM
