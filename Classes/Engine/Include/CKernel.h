#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CNode.h"

namespace LM 
{

class CInputManager;
class CJsonParser;
class CNetworkManager;

/// \class CKernel
/// \ingroup Engine
/// \brief the kernel of the game engine. Containing the behavior tree
class CKernel
{
 private:
  /// \brief the Behavior Tree of the game
  /// \details a pointer to the root node of the tree, usually a SequenceNode
  CNode* m_pBehaviorTree;
  /// \details will forward the inputs to the behavior tree
  CInputManager* m_pInputManager;
  /// \details will manage networking events such as direct wifi
  CNetworkManager* m_pNetworkManager;
  
  /// \brief The parser that will build the behavior tree from the json file
  CJsonParser* m_pJsonParser;

 public:
  CKernel();
  virtual ~CKernel();
  /// \returns the behavior tree
  CNode* GetBehaviorTree();

  /// \brief Initialize the kernel and the behavior tree
  void Init();

  ////////////////// callbacks
  void NavNext(cocos2d::Ref* pSender);
  void NavPrevious(cocos2d::Ref* pSender);

  void SendMessage(cocos2d::Ref* pSender);
  
};


} // namespace LM

#endif /* _CKERNEL_H_ */
