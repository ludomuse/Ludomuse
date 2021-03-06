#include "../Include/CTransitionVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CKernel.h"
#include "../Include/CCallback.h"

#include "../../Modules/Util/Include/CStats.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CTransitionVisitor::CTransitionVisitor(CKernel* a_pKernel, bool a_bTransitionNext) :
	m_bTransitionNext(a_bTransitionNext),
	m_pKernel(a_pKernel)
{
}

Result CTransitionVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  // pSequence is the father node of all the scenes. 
  // transition will be done on the between the current scene in the node 
  // (CSequenceNode::m_iCurrentNode) and the next node
  CSequenceNode* pSequence = dynamic_cast<CSequenceNode*>(a_pNode);
  CSceneNode* pScene = nullptr;
  if (pSequence)
  {
	  pScene = dynamic_cast<CSceneNode*>(pSequence->GetCurrentNode());
  }
  if (pScene)
  {
	  GotoScene(pSequence);
  }
  return RESULT_PRUNE;
}


void CTransitionVisitor::GotoScene(CSequenceNode* a_pSequence)
{
	if (m_pKernel->m_pCurrentScene->m_bDashboardTrigger && m_bTransitionNext && !m_pKernel->m_pLocalPlayer->m_bWaiting)
	{
		// init dashboard
		InitScene(m_pKernel->m_pDashboard);
		return;
	}


	CNode* pNewNode = a_pSequence->GetOffsetNode(m_bTransitionNext);

	if (!pNewNode)
	{
		return;
	}

	CSceneNode* pNewSceneNode = dynamic_cast<CSceneNode*>(pNewNode);
	if (pNewSceneNode)
	{
		if (!m_pKernel->PlayerHasScene(pNewSceneNode->GetSceneID()))
		{ // if the player does not have this scene in his list, skip it and go to the next one
			CCLOG("player do not have this scene, skipping");
			a_pSequence->OffsetCurrentNode(m_bTransitionNext);
			GotoScene(a_pSequence);
			return;
		}
		else if (pNewSceneNode->IsSynced())
		{
			if (!m_bTransitionNext)
			{
				return;
			}

			m_pKernel->m_oSyncMutex.lock();

			if (m_pKernel->m_pDistantPlayer->m_bWaiting)
			{
				CCLOG("distant player waiting : set to not waiting");
				SEvent oMessage;
				oMessage.m_sStringValue = "kernel:waiting";
				m_pKernel->SendNetworkMessage(oMessage, nullptr);

				m_pKernel->m_pDistantPlayer->m_bWaiting = false;
				a_pSequence->OffsetCurrentNode(m_bTransitionNext);
				InitScene(pNewSceneNode);
			}
			else if (m_pKernel->m_pLocalPlayer->m_bWaiting)
			{
				CCLOG("local player waiting : set to not waiting");

				m_pKernel->m_pLocalPlayer->m_bWaiting = false;
				a_pSequence->OffsetCurrentNode(m_bTransitionNext);
				//InitScene(pNewSceneNode);
				LoadInitScene();
			}
			else
			{
				SEvent oMessage;
				oMessage.m_sStringValue = "kernel:waiting";
				m_pKernel->SendNetworkMessage(oMessage, nullptr);
				CCLOG("set local player to waiting");
				// init waiting scene
				m_pKernel->m_pLocalPlayer->m_bWaiting = true;
				InitScene(pNewSceneNode, true);
				m_pKernel->m_oSyncTransitionStart = std::chrono::system_clock::now();
			}

			m_pKernel->m_oSyncMutex.unlock();

		}
		else
		{
			a_pSequence->OffsetCurrentNode(m_bTransitionNext);
			InitScene(pNewSceneNode);
		}
	}

}

void CTransitionVisitor::InitScene(CSceneNode* a_pSceneNode, bool a_bWaitScene)
{
	Scene* pNewScene = a_pSceneNode->CreateScene();
	a_pSceneNode->init();
	pNewScene->retain();
	if (a_bWaitScene)
	{
		Scene* pWaitingScene = m_pKernel->m_pWaitingScene->CreateScene();
		m_pKernel->m_pWaitingScene->init();
		if (m_bTransitionNext)
		{
			Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, pWaitingScene));
		}
		else
		{
			Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, pWaitingScene));
		}
	}
	else {
		if (m_bTransitionNext)
		{
			Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, pNewScene));
		}
		else
		{
			Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, pNewScene));
		}
	}

	CSceneNode* pOldScene = m_pKernel->m_pCurrentScene;
	m_pKernel->m_pCurrentScene = a_pSceneNode;
	if (pOldScene != nullptr)
	{
		pOldScene->UnInit(false);
	}
	/*auto fpUnInitScene = CallFunc::create([pOldScene]() {
		pOldScene->UnInit(false);
	});

	auto oSequence = Sequence::create(DelayTime::create(0.6f), fpUnInitScene, nullptr);
	m_pKernel->m_pCurrentScene->runAction(oSequence);*/
	M_STATS->StartStats();
}

void CTransitionVisitor::LoadInitScene() {
	//if (m_bTransitionNext)
	//{
	//	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, m_pKernel->m_pCurrentScene->GetScene()));
	//}
	//else
	//{
	//	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, m_pKernel->m_pCurrentScene->GetScene()));
	//}
	//Director::getInstance()->popScene();
	Scene* pNewScene = m_pKernel->m_pCurrentScene->GetScene();
	//m_pKernel->m_pCurrentScene->init();
	if (m_bTransitionNext)
	{
		Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, pNewScene));
	}
	else
	{
		Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, pNewScene));
	}
	m_pKernel->m_pWaitingScene->UnInit(false);
}

} // namespace LM
