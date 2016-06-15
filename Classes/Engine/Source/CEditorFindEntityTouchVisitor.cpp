// Include LudoMuse
#include "Classes/Engine/Include/CEditorFindEntityTouchVisitor.h"
#include "Classes/Engine/Include/CLabelNode.h"
#include "Classes/Engine/Include/CSpriteNode.h"

// Include QT
#include <QDebug>

namespace LM{

CEditorFindEntityTouchVisitor::CEditorFindEntityTouchVisitor(CKernel* a_Kernel):
    CFindEntityTouchVisitor(nullptr, Desc<CNode>(), "Touch"),
    m_pEvent(nullptr),
    m_pKernel(a_Kernel),
    m_iTouchID(0)
{

}


CEditorFindEntityTouchVisitor::CEditorFindEntityTouchVisitor(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent, LM::CKernel* a_pKernel) :
    CFindEntityTouchVisitor(a_pTouch, Desc<LM::CNode>(), "Touch"),
    m_pEvent(a_pEvent),
    m_pKernel(a_pKernel),
    m_iTouchID(a_pTouch->getID())
{
CCLOG("CEditorFindEntityTouchVisitor::CEditorFindEntityTouchVisitor touch id : %d", m_iTouchID);
}


bool CEditorFindEntityTouchVisitor::SetVisitor(cocos2d::Event* a_pEvent, cocos2d::Touch* a_pTouch)
{
    this->m_pEvent = a_pEvent;
    this->m_pTouch = a_pTouch;
    this->m_bStopVisiting = false;
    return true;
}


Result CEditorFindEntityTouchVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
    return RESULT_CONTINUE;
}

Result CEditorFindEntityTouchVisitor::ProcessNodeBottomUp(CNode* a_pNode)
{
    //CFindEntityTouchVisitor::ProcessNodeBottomUp(a_pNode);

    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
    if (pEntity)
    {

        cocos2d::Vec2 oTouchLocation = m_pTouch->getStartLocation();
        cocos2d::Rect oBoundingBox = pEntity->GetCocosEntity()->getBoundingBox();
        if (oBoundingBox.containsPoint(oTouchLocation) && !pEntity->IsLocked() && pEntity->IsVisible())
        {
            // forget about touch and move events in the editor :
            CLabelNode* pLabel = dynamic_cast<CLabelNode*>(pEntity);
            if (pLabel && !m_bStopVisiting)
            {
//                m_pKernel->EditTextValue(pLabel);
//                m_pEntityToFind.Set(pLabel);
                m_bStopVisiting = true;
                qDebug()<<"Touchay un Label";
                emit labelClicked(pLabel);
                return RESULT_STOP;

            }
            CSpriteNode* pSprite = dynamic_cast<CSpriteNode*>(pEntity);
            if (pSprite && !m_bStopVisiting)
            {
//                m_pKernel->EditSpritePath(pSprite);
//                m_pEntityToFind.Set(pSprite);
                m_bStopVisiting = true;
                qDebug()<<"Touchay un Sprite";
                emit spriteClicked(pSprite);
                return RESULT_STOP;
            }
        }
    }
    return RESULT_CONTINUE;

}

bool CEditorFindEntityTouchVisitor::SetTouch(cocos2d::Touch* a_pTouch)
{
    this->m_pTouch = a_pTouch;
    return true;
}
bool CEditorFindEntityTouchVisitor::SetEvent(cocos2d::Event* a_pEvent)
{
    this->m_pEvent = a_pEvent;
    return true;
}


}

