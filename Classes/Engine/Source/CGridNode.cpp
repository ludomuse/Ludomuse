#include "../Include/CGridNode.h"


using namespace cocos2d;

namespace LM
{

	CGridNode::CGridNode(int a_iRows, int a_iCols, EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition) :
		CGroupNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
		m_iRows(a_iRows),
		m_iCols(a_iCols),
		m_iColIndex(0),
		m_iRowIndex(0)
{
  
}


	void CGridNode::Init()
	{
        m_iColIndex = 0;
        m_iRowIndex = 0;
		// populate empty slots in the grid with placeholder groups
		for (int iNbPlaceholders = m_iRows * m_iCols - m_vChildren.size();
			iNbPlaceholders > 0;
			--iNbPlaceholders)
		{
			CGroupNode* pPlaceHolder = new CGroupNode();
			AddChildNode(pPlaceHolder);
		}

		CGroupNode::Init();
	}

    void CGridNode::SetGrid(int a_iRows, int a_iCols)
    {
        m_iRows = a_iRows;
        m_iCols = a_iCols;
        ON_CC_THREAD(LM::CGridNode::Update, this);
    }

    int CGridNode::GetRowCount()
    {
        return m_iRows;
    }

    int CGridNode::GetColCount()
    {
        return m_iCols;
    }


cocos2d::Size CGridNode::GetVisibleSize()
{
    cocos2d::Size oGridSize = CEntityNode::GetVisibleSize();
//    if (m_iRowIndex < m_iRows && m_iColIndex < m_iCols)
//    {
        return cocos2d::Size(oGridSize.width / m_iCols, oGridSize.height / m_iRows);
//    }
//    return cocos2d::Size(0, 0);
}

Vec2 CGridNode::GetOrigin()
{
	Vec2 oGridOrigin = CEntityNode::GetOrigin();

	// return the origin of the cell of the grid for the item
    Vec2 oNodeOrigin = Vec2(oGridOrigin.x + m_iColIndex * GetVisibleSize().width,
        oGridOrigin.y + m_iRowIndex * GetVisibleSize().height);

    if (m_iRowIndex < m_iRows && m_iColIndex < m_iCols - 1)
	{
		m_iColIndex++;
	}
	else
	{
		m_iRowIndex++;
		m_iColIndex = 0;
	}

	return oNodeOrigin;
}

void CGridNode::ToJson(rapidjson::Value &a_rParent, rapidjson::Document::AllocatorType &a_rAllocator)
{

    rapidjson::Value gridNode(rapidjson::kObjectType);
    gridNode.AddMember("type", "Grid", a_rAllocator);
    if(!m_sID.empty())
    {
        gridNode.AddMember("id", rapidjson::Value(m_sID.c_str(), m_sID.length()), a_rAllocator);
    }
    rapidjson::Value params(rapidjson::kObjectType);
    params.AddMember("cols", m_iCols, a_rAllocator);
    params.AddMember("rows", m_iRows, a_rAllocator);
    params.AddMember("anchor", m_eAnchor, a_rAllocator);
    params.AddMember("width", m_iWidth, a_rAllocator);
    params.AddMember("height", m_iHeight, a_rAllocator);
    params.AddMember("x", m_iXPosition, a_rAllocator);
    params.AddMember("y", m_iYPosition, a_rAllocator);

    if(!this->m_mListeners.empty())
    {
        rapidjson::Value listeners(rapidjson::kArrayType);
        CEntityNode::ToJsonListener(listeners, a_rAllocator);
        params.AddMember("listeners", listeners, a_rAllocator);
    }

    if(!this->m_vChildren.empty())
    {
        rapidjson::Value children(rapidjson::kArrayType);
        for(CNode* currentNode : this->m_vChildren)
        {
            currentNode->ToJson(children, a_rAllocator);
        }
        params.AddMember("children", children, a_rAllocator);
    }

    gridNode.AddMember("params", params, a_rAllocator);
    a_rParent.PushBack(gridNode, a_rAllocator);
}

} // namespace LM
