#ifndef _CFINDENTITYVISITOR_H_
#define _CFINDENTITYVISITOR_H_

#include "CEntityNode.h"
#include "CVisitor.h"
#include "cocos2d.h"


namespace LM
{


template <typename T>
class Desc {
	T** m_pPointer;
	int* m_iRefCount;

public:
	Desc(T* a_pPointer = nullptr)
	{
		m_pPointer = new T*();
		m_iRefCount = new int();

		*m_pPointer = a_pPointer;
		*m_iRefCount = 1;
	}


	Desc(const Desc& m_rCopyFrom)
	{
		m_pPointer = m_rCopyFrom.m_pPointer;
		m_iRefCount = m_rCopyFrom.m_iRefCount;

		++(*m_iRefCount);
	}

	~Desc()
	{
		--(*m_iRefCount);
		if (*m_iRefCount <= 0)
		{
			delete m_pPointer;
			delete m_iRefCount;
		}
	}


	void Set(T* a_pNewValue)
	{
		*m_pPointer = a_pNewValue;
	}

	T* Get()
	{
		return *m_pPointer;
	}


	bool IsValid()
	{
		return *m_pPointer;
	}
};

class CFindEntityVisitor : public CVisitor
{
 protected:
  std::string m_sEvent;
  Desc<CEntityNode> m_pEntityToFind;

  bool m_bStopVisiting;

 public:
  CFindEntityVisitor(Desc<CEntityNode> a_pEntity,
                     const std::string& a_sEvent);


  virtual void Traverse(CNode* a_pNode) override;

 protected:
  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
  virtual Result ProcessNodeBottomUp(CNode* a_pNode) override;
  
};




class CFindEntityTouchVisitor : public CFindEntityVisitor
{
protected:
	cocos2d::Touch* m_pTouch;


public:
	CFindEntityTouchVisitor(cocos2d::Touch* a_pTouch, Desc<CEntityNode> a_pEntity,
		const std::string& a_sEvent);

	virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
};

} // namespace LM

#endif /* _CFINDENTITYVISITOR_H_ */
