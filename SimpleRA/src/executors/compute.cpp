#include "global.h"
/**
 * @brief
 * SYNTAX: CheckCompute matrix_name
 */
bool syntacticParseCOMPUTE()
{
    logger.log("syntacticParseCOMPUTE");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = COMPUTE;
    parsedQuery.computeMatrixName = tokenizedQuery[1];
    return true;
}

bool semanticParseCOMPUTE()
{
    logger.log("semanticParseCOMPUTE");
    if (parsedQuery.queryType != COMPUTE)
    {
        cout << "SEMANTIC ERROR: Invalid query type" << endl;
        return false;
    }
    if (!tableCatalogue.isTable(parsedQuery.computeMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix does not exist" << endl;
        return false;
    }
    return true;
}

void executeCOMPUTE()
{
    
}