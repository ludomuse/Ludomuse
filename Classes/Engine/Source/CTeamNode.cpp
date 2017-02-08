#include "../Include/CTeamNode.h"
#include <numeric>

namespace LM
{

CTeamNode::CTeamNode(TTasksArray a_oTasksArray,
                     EAnchor a_eAnchor,
                     int a_iWidth,
                     int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_oTasksArray(a_oTasksArray),
	m_iTaskNum(0)
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

}




} // namespace LM
