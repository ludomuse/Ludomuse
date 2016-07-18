#ifndef _STATS_H_
#define _STATS_H_

#include "cocos2d.h"


using namespace std::chrono;

namespace LM
{


struct SScreenStats
{
  int time;

  int nbInteractions;

  int nbTouches;
  int nbValidTouches;

  int nbMoves;

  int nbValidDrops;
  int nbInvalidDrops;
 
  int nbValidAnswers;
  int nbInvalidAnswers;

  SScreenStats();
  void reinit();
  
};


class CStats
{

 private:
  CStats();
  CStats(const CStats&);
  CStats& operator=(const CStats&);

  time_point<system_clock> m_oScreenStartTime;

  ///// stats members
  std::map<std::string, SScreenStats> m_mScreensStats;
  

 public:
  SScreenStats m_oCurrentScreenStats;
  
  static CStats* Instance();
  void PushStats(const std::string& a_rScreenID);
  void StartStats();
  std::map<std::string, SScreenStats> GetStats() const;

};



#include "../../Networking/android/Include/LmBytesMessage.h"

class CSerializableStats : public LmSerializable
{

public:
	std::map<std::string, SScreenStats> m_mScreensStats;


	CSerializableStats(const std::map<std::string, SScreenStats>& a_mStats) : 
		m_mScreensStats(a_mStats) {}

	CSerializableStats() : 
		m_mScreensStats(std::map<std::string, SScreenStats>()){}

	virtual void writeOn(bytes* msg) override
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



	virtual void readOn(bytes* msg) override 
	{
		int iSize;
		*msg >> iSize;

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
			m_mScreensStats.insert(std::pair<std::string, SScreenStats>(screenID, stats));
		}

	}


};


std::ostream& operator<<(std::ostream& os, const CSerializableStats& a_rStats);


} // namespace LM


#define M_STATS CStats::Instance()
#define M_STATS_SCREEN CStats::Instance()->m_oCurrentScreenStats

#endif /* _STATS_H_ */
