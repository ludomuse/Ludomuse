#include "../Include/Util.h"


std::vector<std::string> StringSplit(const std::string& a_sToSplit, char a_cSplitter)
{

  std::vector<std::string> vResult;
  
  // split string
  int iFoundFirst = 0;
  int iFoundNext = a_sToSplit.find_first_of(a_cSplitter);
  while (iFoundNext != std::string::npos)
  {
    vResult.push_back(a_sToSplit.substr(iFoundFirst, iFoundNext - iFoundFirst));
    iFoundFirst = iFoundNext + 1;
    iFoundNext = a_sToSplit.find_first_of(a_cSplitter, iFoundFirst);
  }
  vResult.push_back(a_sToSplit.substr(iFoundFirst, a_sToSplit.size()));

  return vResult;
}
