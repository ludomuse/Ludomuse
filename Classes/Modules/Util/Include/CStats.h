#ifndef _STATS_H_
#define _STATS_H_

#include "cocos2d.h"

namespace LM
{


struct SScreenStats
{
  long time;

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


  ///// stats members
  std::map<std::string, SScreenStats> m_mScreensStats;
  

 public:
  SScreenStats m_oCurrentScreenStats;
  
  static CStats* Instance();
  void PushStats(const std::string& a_rScreenID);
  void StartStats();
};

} // namespace LM


#define M_STATS CStats::Instance()
#define M_STATS_SCREEN CStats::Instance()->m_oCurrentScreenStats

#endif /* _STATS_H_ */
