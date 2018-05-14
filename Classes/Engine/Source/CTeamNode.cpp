#include <numeric>
#include "../Include/CTeamNode.h"
#include "../Include/CLabelNode.h"
#include "../Include/CFindTeamNodeIDVisitor.h"
#include <cocos2d.h>

#ifdef __ANDROID__
#include "../../Modules/Util/Include/FToString.h"
#endif
namespace LM
{

CTeamNode::CTeamNode(TTasksArray a_oTasksArray,
					 CKernel* a_pKernel,
                     EAnchor a_eAnchor,
                     int a_iWidth,
                     int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CGroupNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_oTasksArray(a_oTasksArray),
	m_pKernel(a_pKernel),
	m_iPlayer1CurrentTask(0),
	m_iPlayer2CurrentTask(0)
{


}



void CTeamNode::Init()
{

	CGroupNode::Init();

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
				oActionsIndexes.erase(oActionsIndexes.begin() + iActionRand);
			}
			else // player 2 tasks and actions
			{
				m_oPlayer2Tasks[iPlayer2Index] = m_oTasksArray[oTaskIndexes[iTaskRand]][0];
				oTaskIndexes.erase(oTaskIndexes.begin() + iTaskRand);

				m_oPlayer2Actions[iPlayer2Index++] = m_oTasksArray[oActionsIndexes[iActionRand]][1];
				oActionsIndexes.erase(oActionsIndexes.begin() + iActionRand);
			}
		}


		// send player 2 actions
		std::string sMessage = "kernel:TeamNode:Actions";
		for (int i = 0; i < M_NB_TASK / 2; ++i)
		{
			sMessage += std::string(":") + m_oPlayer2Actions[i];
		}
		m_pKernel->SendNetworkMessage(sMessage);

		// send player 2 current Task
		SendTask(m_oPlayer2Tasks[m_iPlayer2CurrentTask]);


		// update player 1 current task
		UpdateTask(m_oPlayer1Tasks[m_iPlayer1CurrentTask]);

		// update player 1 actions
		UpdateActions(m_oPlayer1Actions);


	}


}


bool CTeamNode::ValidateTask(const std::string& a_rAction)
{
	// find matching task in task/action table
	std::string sMatchingTask = "";
	for (int i = 0; i < M_NB_TASK; ++i)
	{
		if (m_oTasksArray[i][1] == a_rAction)
		{
			sMatchingTask = m_oTasksArray[i][0];
			break;
		}
	}
	if (sMatchingTask == "")
	{
		return false;
	}


	// find player having the task and increment task
	if (m_iPlayer1CurrentTask < M_NB_TASK / 2 && m_oPlayer1Tasks[m_iPlayer1CurrentTask] == sMatchingTask)
	{
		++m_iPlayer1CurrentTask;
		if (m_iPlayer1CurrentTask >= M_NB_TASK / 2)
		{

			TasksFinished();

			SEvent oEvent;
			oEvent.m_sStringValue = "Player1";
			m_pKernel->Validate(oEvent, nullptr);

		}
		else
		{
			UpdateTask(m_oPlayer1Tasks[m_iPlayer1CurrentTask]);
		}

		return true;
	}

	else if (m_iPlayer2CurrentTask < M_NB_TASK / 2 && m_oPlayer2Tasks[m_iPlayer2CurrentTask] == sMatchingTask)
	{
		m_iPlayer2CurrentTask++;
		if (m_iPlayer2CurrentTask >= M_NB_TASK / 2)
		{

			m_pKernel->SendNetworkMessage("kernel:TeamNode:TeamTasksFinished");

			SEvent oEvent;
			oEvent.m_sStringValue = "Player2";
			m_pKernel->Validate(oEvent, nullptr);

		}
		else
		{
			SendTask(m_oPlayer2Tasks[m_iPlayer2CurrentTask]);
		}

		return true;
	}

	return false;
}



void CTeamNode::UpdateTask(const std::string& a_rNextTask)
{
	Desc<CNode> oFoundEntity;
	CFindTeamNodeIDVisitor oVisitor(oFoundEntity, "TeamNode:Task");
	oVisitor.Traverse(m_pKernel->m_pBehaviorTree);
	if (oFoundEntity.IsValid())
	{
		CLabelNode* pLabelNode = static_cast<CLabelNode*>(oFoundEntity.Get());
		pLabelNode->SetText(a_rNextTask);
	}
}


void CTeamNode::UpdateActions(const std::array<std::string, M_NB_TASK / 2>& a_rActions)
{
	for (int i = 0; i < M_NB_TASK / 2; ++i)
	{
		Desc<CNode> oFoundEntity;
		CFindTeamNodeIDVisitor oVisitor(oFoundEntity, std::string("TeamNode:Action") + std::to_string(i + 1));
        oVisitor.Traverse(m_pKernel->m_pCurrentScene);
		if (oFoundEntity.IsValid())
		{
			CLabelNode* pLabelNode = static_cast<CLabelNode*>(oFoundEntity.Get());
			pLabelNode->SetText(a_rActions[i]);
		}
	}
}


void CTeamNode::TasksFinished()
{
	Desc<CNode> oFoundEntity;

	CFindTeamNodeIDVisitor oVisitor(oFoundEntity, "TeamNode:Task");
	oVisitor.Traverse(m_pKernel->m_pBehaviorTree);
	if (oFoundEntity.IsValid())
	{
		CLabelNode* pLabelNode = static_cast<CLabelNode*>(oFoundEntity.Get());
		pLabelNode->Dispatch("TeamTasksFinished");
	}
}

#ifdef LUDOMUSE_EDITOR
TTasksArray CTeamNode::GetTasks()
{
    return m_oTasksArray;
}

void CTeamNode::SetTasks(TTasksArray a_oTasks)
{
    m_oTasksArray = a_oTasks;
}

void CTeamNode::SetTask(int a_iIndex, const std::string &a_rTask)
{
    m_oTasksArray[a_iIndex][0] = a_rTask;
}

void CTeamNode::SetAction(int a_iIndex, const std::string &a_rAction)
{
    m_oTasksArray[a_iIndex][1] = a_rAction;
}

#endif
// send new task to remote player 
void CTeamNode::SendTask(const std::string& a_rNextTask)
{
	m_pKernel->SendNetworkMessage(std::string("kernel:TeamNode:NewTask:") + a_rNextTask);
}

#ifdef LUDOMUSE_EDITOR
void CTeamNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{
    rapidjson::Value teamNode(rapidjson::kObjectType);
    teamNode.AddMember("type", "TeamNode", a_rAllocator);

    if (!m_sID.empty())
    {
        teamNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }

    rapidjson::Value params(rapidjson::kObjectType);

    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

    rapidjson::Value tasks(rapidjson::kArrayType);
    for (int i = 0; i < M_NB_TASK; ++i)
    {
        rapidjson::Value taskAction(rapidjson::kArrayType);
        taskAction.PushBack(rapidjson::Value(m_oTasksArray[i][0].c_str(), m_oTasksArray[i][0].length()), a_rAllocator); // task string
        taskAction.PushBack(rapidjson::Value(m_oTasksArray[i][1].c_str(), m_oTasksArray[i][1].length()), a_rAllocator); // action string

        tasks.PushBack(taskAction, a_rAllocator);
    }

    params.AddMember("tasks", tasks, a_rAllocator);

    if (!m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if (!m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    teamNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(teamNode, a_rAllocator);
}
#endif


} // namespace LM
