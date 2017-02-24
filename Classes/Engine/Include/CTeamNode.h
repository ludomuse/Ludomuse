#ifndef _CTEAMNODE_H_
#define _CTEAMNODE_H_

#include "CGroupNode.h"
#include "CKernel.h"
#include <array>


#define M_NB_TASK 8

namespace LM
{

typedef std::array<std::array<std::string, 2>, M_NB_TASK> TTasksArray;


class CTeamNode : public CGroupNode
{

 private:
  TTasksArray m_oTasksArray;

  CKernel* m_pKernel;

  std::array<std::string, M_NB_TASK / 2> m_oPlayer1Tasks;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer2Tasks;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer1Actions;
  std::array<std::string, M_NB_TASK / 2> m_oPlayer2Actions;

  int m_iPlayer1CurrentTask;
  int m_iPlayer2CurrentTask;

 public:
  CTeamNode(TTasksArray a_oTasksArray,
			CKernel* a_pKernel,
            EAnchor a_eAnchor,
            int a_iWidth,
            int a_iHeight,
            int a_iXPosition,
            int a_iYPosition);

  void Init() override;

  bool ValidateTask(const std::string& a_rAction);
  void UpdateTask(const std::string& a_rNextTask);
  void UpdateActions(const std::array<std::string, M_NB_TASK / 2>& a_rActions);
  void TasksFinished();

private:
	void SendTask(const std::string& a_rNextTask);

};

} // namespace LM

#endif /* _CTEAMNODE_H_ */
