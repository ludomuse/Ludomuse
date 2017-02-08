#ifndef _CTEAMNODE_H_
#define _CTEAMNODE_H_

#include "CEntityNode.h"
#include <array>


#define M_NB_TASK 8

namespace LM
{

typedef std::array<std::array<std::string, 2>, M_NB_TASK> TTasksArray;


class CTeamNode : public CEntityNode
{

 private:
  TTasksArray m_oTasksArray;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer1Tasks;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer2Tasks;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer1Actions;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer2Actions;

  int m_iTaskNum;

 public:
  CTeamNode(TTasksArray a_oTasksArray,
            EAnchor a_eAnchor,
            int a_iWidth,
            int a_iHeight,
            int a_iXPosition,
            int a_iYPosition);
  
  
};

} // namespace LM

#endif /* _CTEAMNODE_H_ */
