#include "../Include/Util.h"


namespace LM
{

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


    std::string basename(const std::string& a_rPath)
    {
        std::vector<std::string> oSplittedString = StringSplit(a_rPath, '/');
        return oSplittedString[oSplittedString.size() - 1];
    }

} // namespace LM
