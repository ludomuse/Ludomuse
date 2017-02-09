#include <numeric>
#include "../Include/CTeamNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CFindEntityFromIDVisitor.h"


namespace LM
{

CTeamNode::CTeamNode(TTasksArray a_oTasksArray,
					 CKernel* a_pKernel,
                     EAnchor a_eAnchor,
                     int a_iWidth,
                     int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_oTasksArray(a_oTasksArray),
	m_pKernel(a_pKernel),
	m_iPlayer1CurrentTask(0),
	m_iPlayer2CurrentTask(0)
{

	if (m_pKernel->m_pLocalPlayer->m_iPlayerID == 0)
	{


		// do only following for "master" player 

		std::vector<int> oTaskIndexes(M_NB_TASK);
		std::iota(oTaskIndexes.begin(), oTaskIndexes.end(), 0); // populate vector with increasing int

		std::vector<int> oActionsIndexes(oTaskIndexes);

		// initialize random seed
		std::srand(time(NULL));


		int iPlayer1Index = 0, iPlayer2Index = 0;
		for (int count = M_NB_TASK; count > 0; --count)
		{
			int iTaskRand = std::rand() % count;
			int iActionRand = std::rand() % count;

			if (count > M_NB_TASK / 2) // player 1 tasks and actions
			{
				m_oPlayer1Tasks[iPlayer1Index] = m_oTasksArray[oTaskIndexes[iTaskRand]][0];
				oTaskIndexes.erase(oTaskIndexes.begin() + iTaskRand);

				m_oPlayer1Actions[iPlayer1Index++] = m_oTasksArray[oActionsIndexes[iActionRand]][1];
				oActionsIndexes.erase(oActionsIndexes.begin() + iTaskRand);
			}
			else // player 2 tasks and actions
			{
				m_oPlayer2Tasks[iPlayer2Index] = m_oTasksArray[oTaskIndexes[iTaskRand]][0];
				oTaskIndexes.erase(oTaskIndexes.begin() + iTaskRand);

				m_oPlayer2Actions[iPlayer2Index++] = m_oTasksArray[oActionsIndexes[iActionRand]][1];
				oActionsIndexes.erase(oActionsIndexes.begin() + iTaskRand);
			}
		}

		// update player 1 current task
		UpdateTask(m_oPlayer1Tasks[m_iPlayer1CurrentTask]);

		// update player 1 actions
		UpdateActions(m_oPlayer1Actions);

		// send player 2 actions
		std::string sMessage = "kernel:TeamNode:Actions";
		for (int i = 0; i < M_NB_TASK / 2; ++i)
		{
			sMessage += std::string(":") + m_oPlayer2Actions[i];
		}
		m_pKernel->SendNetworkMessage(sMessage);

		// send player 2 current Task
		SendTask();
	}


}



bool CTeamNode::ValidateTask(int a_iTaskNum)
{
	if (a_iTaskNum == m_iPlayer1CurrentTask)
	{
		++m_iPlayer1CurrentTask;
		if (m_iPlayer1CurrentTask >= M_NB_TASK / 2)
		{
			SEvent oEvent;
			oEvent.m_sStringValue = "Player1";
			m_pKernel->Validate(oEvent, nullptr);
		}
		else
		{
			UpdateTask(m_oPlayer1Tasks[++m_iPlayer1CurrentTask]);
		}
	
	}
	else if (a_iTaskNum == m_iPlayer2CurrentTask)
	{
		m_iPlayer2CurrentTask++;
		if (m_iPlayer2CurrentTask >= M_NB_TASK / 2)
		{
			SEvent oEvent;
			oEvent.m_sStringValue = "Player2";
			m_pKernel->Validate(oEvent, nullptr);
		}
		else
		{
			SendTask();
		}
	}
}



void CTeamNode::UpdateTask(const std::string& a_sNextTask)
{
	Desc<CNode> oFoundEntity;
	CFindEntityFromIDVisitor oVisitor(oFoundEntity, "TeamNode:Task");
	if (oFoundEntity.IsValid())
	{
		CLabelNode* pLabelNode = static_cast<CLabelNode*>(oFoundEntity.Get());
		pLabelNode->SetText(a_sNextTask);
	}
}


void CTeamNode::UpdateActions(const std::array<std::string, M_NB_TASK / 2>& a_rActions)
{
	for (int i = 0; i < M_NB_TASK / 2; ++i)
	{
		Desc<CNode> oFoundEntity;
		CFindEntityFromIDVisitor oVisitor(oFoundEntity, std::string("TeamNode:Action") + std::to_string(i));
		if (oFoundEntity.IsValid())
		{
			CLabelNode* pLabelNode = static_cast<CLabelNode*>(oFoundEntity.Get());
			pLabelNode->SetText(a_rActions[i]);
		}
	}
}



// send new task to remote player 
void CTeamNode::SendTask()
{
	m_pKernel->SendNetworkMessage(std::string("kernel:TeamNode:NewTask:") + m_oPlayer2Tasks[m_iPlayer2CurrentTask]);
}


} // namespace LM
