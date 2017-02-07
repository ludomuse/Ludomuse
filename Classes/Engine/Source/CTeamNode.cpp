#include "../Include/CTeamNode.h"


namespace LM
{

CTeamNode::CTeamNode(TTasksArray a_oTasksArray,
                     EAnchor a_eAnchor,
                     int a_iWidth,
                     int a_iHeight,
                     int a_iXPosition,
                     int a_iYPosition) :
    CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
    m_oTasksArray(a_oTasksArray)
{}




} // namespace LM
