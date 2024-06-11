#include "global.h"
/**
 * @brief
 * SYNTAX: R <- ORDER BY column_name ASC|DESC ON relation_name
 */
bool syntacticParseORDERBY()
{
    logger.log("syntacticParseORDERBY");
    if (tokenizedQuery.size() != 8 || tokenizedQuery[2] != "ORDER"  || tokenizedQuery[3] != "BY" || tokenizedQuery[6] != "ON")
    {
        cout << "SYNTAX ERROR IN TOKEN" << endl;
        return false;
    }

    parsedQuery.queryType = ORDER_BY;
    parsedQuery.orderByResultTableName = tokenizedQuery[0];
    parsedQuery.orderByColumnName = tokenizedQuery[4];
    parsedQuery.orderByTableName = tokenizedQuery[7];

    string sortingStrategy = tokenizedQuery[5];
    if (sortingStrategy == "ASC")
        parsedQuery.orderBySortingStrategy = ASC;
    else if (sortingStrategy == "DESC")
        parsedQuery.orderBySortingStrategy = DESC;
    else
    {
        cout << "SYNTAX ERROR IN SORTING STRATEGY" << endl;
        return false;
    }

    return true;
}

bool semanticParseORDERBY()
{
    logger.log("semanticParseORDERBY");

    if (tableCatalogue.isTable(parsedQuery.orderByResultTableName))
    {
        cout << "SEMANTIC ERROR: Resultant table already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.orderByTableName))
    {
        cout << "SEMANTIC ERROR: Table doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.orderByColumnName, parsedQuery.orderByTableName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in table" << endl;
        return false;
    }

    return true;
}

void executeORDERBY()
{
    logger.log("executeORDERBY");

    Table *oldTable = tableCatalogue.getTable(parsedQuery.orderByTableName);
    vector<SortingStrategy> strtgy;
    strtgy.push_back(parsedQuery.orderBySortingStrategy);
    vector<string> columnName;
    columnName.push_back(parsedQuery.orderByColumnName);
 
    // Sort the table according to the column name and sorting strategy provided
    string str = oldTable->sortTable(columnName, strtgy);
    
    for (int i = 0; i < oldTable->blockCount; i++)
    {
        string pgOldName = "../data/temp/"+ str + "_Page" + to_string(i);
        string pgNewName = "../data/temp/"+ parsedQuery.orderByResultTableName + "_Page" + to_string(i);
        rename(pgOldName.c_str(),pgNewName.c_str());
    }
    
    Table *resultantTable = new Table(parsedQuery.orderByResultTableName, oldTable->columns, false);
    
    resultantTable->distinctValuesPerColumnCount = oldTable->distinctValuesPerColumnCount;
    resultantTable->rowCount = oldTable->rowCount;
    resultantTable->blockCount = oldTable->blockCount;
    resultantTable->rowsPerBlockCount = oldTable->rowsPerBlockCount;
    
    tableCatalogue.insertTable(resultantTable);
    resultantTable->makePermanent();
    return;
}
