#ifndef CEDITORFINDENTITYTOUCHVISITOR_H
#define CEDITORFINDENTITYTOUCHVISITOR_H

#include "Classes/Engine/Include/CFindEntityTouchVisitor.h"
#include "CNode.h"
#include "CLabelNode.h"
#include "CSpriteNode.h"
#include "CTeamNode.h"


#include <QObject>

namespace LM{

class CEditorFindEntityTouchVisitor :  public QObject, public CFindEntityTouchVisitor
{
    Q_OBJECT
public:
    CEditorFindEntityTouchVisitor(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent, CKernel* a_pKernel);
    CEditorFindEntityTouchVisitor(CKernel* a_Kernel = nullptr);

    bool SetVisitor(cocos2d::Event*, cocos2d::Touch*);
    bool SetTouch(cocos2d::Touch*);
    bool SetEvent(cocos2d::Event*);

signals:
    void labelClicked(LM::CLabelNode*);
    void spriteClicked(LM::CSpriteNode*);
    void teamNodeClicked(LM::CTeamNode*);

protected:
    virtual Result ProcessNodeTopDown(CNode* a_pNode) override;
    virtual Result ProcessNodeBottomUp(CNode* a_pNode) override;

private:

    cocos2d::Event* m_pEvent;

    cocos2d::Touch* m_pTouch;

    CKernel* m_pKernel;

    std::string m_sListenEvent;

    bool m_bStopVisiting;

    int m_iTouchID;



};

}

#endif // CEDITORFINDENTITYTOUCHVISITOR_H
