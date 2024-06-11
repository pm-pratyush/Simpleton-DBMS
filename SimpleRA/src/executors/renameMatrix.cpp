#include "global.h"
/**
 * @brief 
 * SYNTAX: RENAME MATRIX o_name n_name
 */
bool syntacticParseRENAME_MATRIX()
{
    logger.log("syntacticParseRENAME_MATRIX");
    if (tokenizedQuery.size() != 4)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = RENAME_MATRIX;
    parsedQuery.renameMatrixOldName = tokenizedQuery[2];
    parsedQuery.renameMatrixNewName = tokenizedQuery[3];
    return true;
}

bool semanticParseRENAME_MATRIX()
{
    logger.log("semanticParseRENAME_MATRIX");
    if (!tableCatalogue.isTable(parsedQuery.renameMatrixOldName))
    {
        cout << "SEMANTIC ERROR: No such matrix exist" << endl;
        return false;
    }
    if (tableCatalogue.isTable(parsedQuery.renameMatrixNewName))
    {
        cout << "SEMANTIC ERROR: Matrix with new name already exist" << endl;
        return false;
    }
    
    return true;
}

void executeRENAME_MATRIX()
{
    logger.log("executeRENAME_MATRIX");
    tableCatalogue.renameTable(parsedQuery.renameMatrixOldName, parsedQuery.renameMatrixNewName);    
    cout << "Matrix Renamed from " << parsedQuery.renameMatrixOldName << " to " << parsedQuery.renameMatrixNewName << endl;

    return;
}