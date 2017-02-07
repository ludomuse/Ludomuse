#ifndef _CTEAMNODE_H_
#define _CTEAMNODE_H_

#include "CEntityNode.h"


namespace LM
{

typedef std::array<std::array<std::string, 2>, 4> TTasksArray;


class CTeamNode : public CEntityNode
{

 private:
  TTasksArray m_oTasksArray;

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
