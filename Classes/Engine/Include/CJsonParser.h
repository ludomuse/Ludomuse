#ifndef _CJSONPARSER_H_
#define _CJSONPARSER_H_

#include <string>
#include "CNode.h"

namespace LM
{

class CJsonParser
{
 public:
  static void BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename);
};

} // namespace LM

#endif /* _CJSONPARSER_H_ */
