#ifndef _CTRANSITIONVISITOR_H_
#define _CTRANSITIONVISITOR_H_

#include "CVisitor.h"
#include "CSequenceNode.h"
#include "CSceneNode.h"

namespace LM
{

class CKernel;

typedef cocos2d::TransitionScene* (*FPTransitionCreate)(float, cocos2d::Scene*);

class CTransitionVisitor : public CVisitor
{
private:
	CKernel* m_pKernel;
	bool m_bTransitionNext;

 public:
	 CTransitionVisitor(CKernel* a_pKernel, bool a_bTransitionNext);

  virtual Result ProcessNodeTopDown(CNode* a_pNode) override;


private:
	void GotoScene(CSequenceNode* a_pSequence);
	#ifdef LUDOMUSE_EDITOR
	void InitScene(CSceneNode* a_pScene);
	#else
	void InitScene(CSceneNode* a_pScene, bool a_bWaitScene = false);
	void LoadInitScene();
	#endif

};


} // namespace LM

#endif /* _CTRANSITIONVISITOR_H_ */
