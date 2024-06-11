#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE MATRIX matrix_name
 */
bool syntacticParseTRANSPOSE_MATRIX()
{
    logger.log("syntacticParseTRANSPOSE_MATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE_MATRIX;
    parsedQuery.transposeMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseTRANSPOSE_MATRIX()
{
    logger.log("semanticParseTRANSPOSE_MATRIX");
    if (tableCatalogue.isTable(parsedQuery.transposeMatrixName))
    {
        return true;
    }
    cout << "SEMANTIC ERROR: Matrix does not exist" << endl;
    return false;
}

void executeTRANSPOSE_MATRIX()
{
    logger.log("executeTRANSPOSE_MATRIX");

    Table* table = tableCatalogue.getTable(parsedQuery.transposeMatrixName);
    table->transpose();
    
    
    return;
}