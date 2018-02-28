#ifndef _CNODE_H_
#define _CNODE_H_

#include <vector>
#include "cocos2d.h"

#ifdef LUDOMUSE_EDITOR
// Include for Json conversion
#include "rapidjson.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"
#endif

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

  virtual void AddChildNodeAtBegin(CNode* a_pChild);

  /// \brief add the node after a node with specific id (use by behavior tree
  void AddChildNodeAt(CNode* a_pChild, const std::string& a_rID);

  void DeleteChildByID(const std::string& a_sID);
  CNode* FindChildByID(const std::string& a_sID, bool a_bRecSearch = false);

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

  virtual bool hasID(const std::string& a_rID);

  virtual std::string GetSceneID();

  virtual CNode* GetOffsetNode(bool a_bNext);

  virtual std::vector<CNode*> GetChildren();

  virtual void SetChildren(const std::vector<CNode*>& a_vChildren);

  virtual CNode* GetParent();

  virtual void SetParent(CNode* a_pParent);

  virtual void Init();

  virtual void UnInit( bool removeChild = true);
#ifdef LUDOMUSE_EDITOR
  virtual void ToJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator);

  virtual bool UseFile(const std::string &a_sFilename);
#endif
};


} // namespace LM

#endif /* _CNODE_H_ */
