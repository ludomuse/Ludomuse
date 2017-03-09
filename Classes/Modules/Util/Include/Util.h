#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>

namespace LM
{

	std::vector<std::string> StringSplit(const std::string& a_sToSplit, char a_cSplitter = ':');
	std::string basename(const std::string& a_rPath);

} // namespace LM
#endif /* _UTIL_H_ */
