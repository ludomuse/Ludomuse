#ifndef _CNODE_H_
#define _CNODE_H_

#include <vector>
#include "cocos2d.h"


namespace LM
{


/// \class CNode
/// \ingroup Engine
/// \brief A simple node of the BehaviorTree
class CNode
{

 public:
  typedef std::vector<CNode*>::iterator Iterator;
  
 protected:
  /// \brief the list of children of the node
  /// \details constitutes the subtree taking this node as root
  std::vector<CNode*> m_vChildren;
  /// \brief the parent node in the tree
  /// \details nullptr if root node
  CNode* m_pParent;
  /// \brief the current child node in the execution process
  int m_iCurrentNode;

 public:
  CNode();
  /// \brief delete all the children nodes
  virtual ~CNode();

  /// \brief Cocos2d-x Callback called when the screen is touched
  /// \returns true on success, false otherwise
  /// \param[in] a_pTouch the cocos2d touch event
  /// \param[in] a_pEvent the cocos2d event
  virtual bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);

  /// \brief add this node as a child in the subtree
  /// \param[in] a_pChild The child to add
  virtual void AddChildNode(CNode* a_pChild);

  CNode::Iterator begin();
  CNode::Iterator end();

  virtual CNode* operator[](const int i);

  //// \brief execute for each child node a_fpCallback(a_pChild)
  // INFO:TMP
  /*virtual void DoForEachChildNode(void (*a_fpCallback)(CNode* a_pChild));

  template<class T>
  void DoForEachChildNode(T* a_tObject, void (T::* a_fpCallback)(CNode* ))
  {
	  for (CNode::Iterator itChild = BeginChild(); itChild != EndChild(); ++itChild)
	  {
		  (a_tObject->*a_fpCallback)(*itChild);
	  }
  };*/

  /// \brief call this function to finish the execution of the node
  virtual void Finish();

};


} // namespace LM

#endif /* _CNODE_H_ */
