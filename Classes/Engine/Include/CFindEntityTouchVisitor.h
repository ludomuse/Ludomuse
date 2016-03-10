#ifndef _CFINDENTITYTOUCHVISITOR_H_
#define _CFINDENTITYTOUCHVISITOR_H_

#include "CFindEntityVisitor.h"

namespace LM
{


class CFindEntityTouchVisitor : public CFindEntityVisitor
{
protected:
	cocos2d::Touch* m_pTouch;


public:
	CFindEntityTouchVisitor(cocos2d::Touch* a_pTouch, const Desc<CEntityNode>& a_pEntity,
		const std::string& a_sEvent);

	virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
};


} // namespace LM

#endif /* _CFINDENTITYTOUCHVISITOR_H_ */
