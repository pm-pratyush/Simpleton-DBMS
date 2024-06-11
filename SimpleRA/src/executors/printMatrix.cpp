#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT MATRIX matrix_name
 */
bool syntacticParsePRINT_MATRIX()
{
    logger.log("syntacticParsePRINT_MATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINT_MATRIX;
    parsedQuery.printRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParsePRINT_MATRIX()
{
    logger.log("semanticParsePRINT_MATRIX");
    if (!tableCatalogue.isTable(parsedQuery.printRelationName))
    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        return false;
    }
    return true;
}

void executePRINT_MATRIX()
{
    logger.log("executePRINT_MATRIX");
    Table* table = tableCatalogue.getTable(parsedQuery.printRelationName);
    table->print();
    return;
}
