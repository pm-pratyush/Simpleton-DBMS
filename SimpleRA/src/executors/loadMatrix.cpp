#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD MATRIX matrix_name
 */
bool syntacticParseLOAD_MATRIX()
{
    logger.log("syntacticParseLOAD_MATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LOAD_MATRIX;
    parsedQuery.loadRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParseLOAD_MATRIX()
{
    logger.log("semanticParseLOAD_MATRIX");
    if (tableCatalogue.isTable(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeLOAD_MATRIX()
{
    logger.log("executeLOAD_MATRIX");

    Table *table = new Table(parsedQuery.loadRelationName, true);
    if (table->load())
    {
        tableCatalogue.insertTable(table);

        if (table->isMatrix)
            cout << "Loaded Matrix." << " Dimension: " << table->rowCount << " * " << table->rowCount << endl;
        else
            cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    }
    return;
}