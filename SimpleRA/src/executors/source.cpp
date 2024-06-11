#include "global.h"
/**
 * @brief 
 * SYNTAX: SOURCE filename
 */
bool syntacticParseSOURCE()
{
    logger.log("syntacticParseSOURCE");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = SOURCE;
    parsedQuery.sourceFileName = tokenizedQuery[1];
    return true;
}

bool semanticParseSOURCE()
{
    logger.log("semanticParseSOURCE");
    if (!isQueryFile(parsedQuery.sourceFileName))
    {
        cout << "SEMANTIC ERROR: File doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeSOURCE()
{
    logger.log("executeSOURCE");

    std::ifstream queryFile;
    queryFile.open("../data/" + parsedQuery.sourceFileName + ".ra");
    if (!queryFile.is_open() ){
        std::cerr << "Error opening .ra file: " << parsedQuery.sourceFileName << std::endl;
        return;
    }
    regex delim("[^\\s,]+");

    std::string command;
    while (!queryFile.eof()) {
        getline(queryFile, command);
        if(command.size()<=0){return;}

        tokenizedQuery.clear();
        parsedQuery.clear();
        logger.log("\nReading New Command: ");
        logger.log(command);

        auto words_begin = std::sregex_iterator(command.begin(), command.end(), delim);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            tokenizedQuery.emplace_back((*i).str());

        if (tokenizedQuery.size() == 1 && tokenizedQuery.front() == "QUIT") {
            break;
        }

        if (tokenizedQuery.empty()) {
            continue;
        }

        if (tokenizedQuery.size() == 1) {
            cout << "SYNTAX ERROR" << endl;
            continue;
        }

        logger.log("doCommand");
        if (syntacticParse() && semanticParse()) {
            executeCommand();
        }
    }
    queryFile.close();

    return;
}
