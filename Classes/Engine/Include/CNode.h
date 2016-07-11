#ifndef _CNODE_H_
#define _CNODE_H_

#include <vector>
#include "cocos2d.h"

// Include for Json conversion
#include "rapidjson.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"

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


  /// \brief add this node as a child in the subtree
  /// \param[in] a_pChild The child to add
  virtual void AddChildNode(CNode* a_pChild);

  /// \brief add the node at after a node with specific id
  void AddChildNodeAt(CNode* a_pChild, const std::string& a_rID);
  // begin and en in order to iterate node through foreach loop
  CNode::Iterator begin();
  CNode::Iterator end();

  virtual CNode* operator[](const int i);


  /// \brief call this function to finish the execution of the node
  virtual void Finish();
  /// \brief returns the currently executed node in the sequence
  virtual CNode* GetCurrentNode();

  virtual bool OffsetCurrentNode(bool a_bNext);

  virtual bool SetCurrentNode(CNode* a_pNode);

  virtual bool isSceneID(const std::string& a_rID);

  virtual CNode* GetOffsetNode(bool a_bNext);

  virtual std::vector<CNode*> GetChildren();

  virtual CNode* GetParent();

  virtual void Init();

  virtual void UnInit( bool removeChild = true);

  virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);

};


} // namespace LM

#endif /* _CNODE_H_ */
