#include "../Include/CGridNode.h"


using namespace cocos2d;

namespace LM
{

	CGridNode::CGridNode(int a_iRows, int a_iCols, EAnchor a_eAnchor, int a_iWidth, int a_iHeight, int a_iXPosition, int a_iYPosition) :
		CEntityNode(a_eAnchor, a_iWidth, a_iHeight, a_iXPosition, a_iYPosition),
		m_iRows(a_iRows),
		m_iCols(a_iCols),
		m_iColIndex(0),
		m_iRowIndex(0)
{
  
}


void CGridNode::Init()
{
	m_pCocosEntity = Node::create();
	PopulateParent(false);

	Size oVisibleSize = GetParentVisibleSize();

	float fNewWidth = oVisibleSize.width * ((float)m_iWidth / 100.0f);
	float fNewHeight = oVisibleSize.height * ((float)m_iHeight / 100.0f);

	m_pCocosEntity->setContentSize(Size(fNewWidth, fNewHeight));


	CNode::Init();
}


Size CGridNode::GetVisibleSize()
{
	Size oGridSize = CEntityNode::GetVisibleSize();
	return Size(oGridSize.width / m_iCols, oGridSize.height / m_iRows);
}

Vec2 CGridNode::GetOrigin()
{
	Vec2 oGridOrigin = CEntityNode::GetOrigin();

	// return the origin of the cell of the grid for the item
	Vec2 oNodeOrigin = Vec2(oGridOrigin.x + m_iColIndex * GetVisibleSize().width,
		oGridOrigin.y + m_iRowIndex * GetVisibleSize().height);

	if (m_iColIndex < m_iCols - 1)
	{
		m_iColIndex++;
	}
	else
	{
		m_iRowIndex++;
		m_iColIndex = 0;
	}

	/// TODO : WARNING may bug with incomplete grids
	if (m_iRowIndex >= m_iRows)
	{
		m_iRowIndex = 0;
	}

	return oNodeOrigin;
}

} // namespace LM
