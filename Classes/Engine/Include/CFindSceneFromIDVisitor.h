#ifndef CFINDSCENEFROMIDVISITOR_H
#define CFINDSCENEFROMIDVISITOR_H


#include "CVisitor.h"
#include "CKernel.h"
#include "CSceneNode.h"

namespace LM{

class CFindSceneFromIDVisitor : public CVisitor
{
private:
 std::string m_sSceneID;
 CKernel* m_pKernel;
 CSceneNode* m_pScene;

public:
 CFindSceneFromIDVisitor(const std::string& a_rSceneID, CKernel* a_pKernel);

 virtual Result ProcessNodeTopDown(CNode* a_pNode) override;

 bool SceneFound();

 CSceneNode* GetSceneFound();


};

}// End namespace LM

#endif // CFINDSCENEFROMIDVISITOR_H
