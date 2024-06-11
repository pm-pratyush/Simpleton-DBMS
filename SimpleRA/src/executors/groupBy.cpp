#include "global.h"
/**
 * @brief
 * SYNTAX: <new_table> <- GROUP BY <grouping_attribute> FROM <table_name> HAVING <aggregate(attribute)> <bin_op> <attribute_value> RETURN <aggregate_func(attribute)>
 */
bool syntacticParseGROUPBY()
{
    logger.log("syntacticParseGROUPBY");
    if (tokenizedQuery.size() != 13 || tokenizedQuery[3] != "BY" || tokenizedQuery[5] != "FROM" || tokenizedQuery[7] != "HAVING" || tokenizedQuery[11] != "RETURN")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = GROUP_BY;
    parsedQuery.groupByResultTableName = tokenizedQuery[0];
    parsedQuery.groupingAttributeName = tokenizedQuery[4];
    parsedQuery.groupByTableName = tokenizedQuery[6];
    parsedQuery.havingValue = tokenizedQuery[10];
    
    parsedQuery.havingAggregateFunctionColumn = tokenizedQuery[8];
    parsedQuery.returnAggregateFunctionColumn = tokenizedQuery[12];

    string havingOperator = tokenizedQuery[9];
    if (havingOperator == "<")
        parsedQuery.havingOperator = LESS_THAN;
    else if (havingOperator == ">")
        parsedQuery.havingOperator = GREATER_THAN;
    else if (havingOperator == "<=")
        parsedQuery.havingOperator = LEQ;
    else if (havingOperator == ">=")
        parsedQuery.havingOperator = GEQ;
    else if (havingOperator == "==")
        parsedQuery.havingOperator = EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    string havingAggregateFunctionColumn = tokenizedQuery[8];
    if (havingAggregateFunctionColumn.substr(0, 3) == "MIN")
        parsedQuery.havingAggregateFunction = MIN;
    else if (havingAggregateFunctionColumn.substr(0, 3) == "MAX")
        parsedQuery.havingAggregateFunction = MAX;
    else if (havingAggregateFunctionColumn.substr(0, 3) == "SUM")
        parsedQuery.havingAggregateFunction = SUM;
    else if (havingAggregateFunctionColumn.substr(0, 3) == "AVG")
        parsedQuery.havingAggregateFunction = AVG;
    else if (havingAggregateFunctionColumn.substr(0, 3) == "CNT")
        parsedQuery.havingAggregateFunction = CNT;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.havingAggregateColumn = havingAggregateFunctionColumn.substr(4, havingAggregateFunctionColumn.size() - 5);

    string returnAggregateFunctionColumn = tokenizedQuery[12];
    if (returnAggregateFunctionColumn.substr(0, 3) == "MIN")
        parsedQuery.returnAggregateFunction = MIN;
    else if (returnAggregateFunctionColumn.substr(0, 3) == "MAX")
        parsedQuery.returnAggregateFunction = MAX;
    else if (returnAggregateFunctionColumn.substr(0, 3) == "SUM")
        parsedQuery.returnAggregateFunction = SUM;
    else if (returnAggregateFunctionColumn.substr(0, 3) == "AVG")
        parsedQuery.returnAggregateFunction = AVG;
    else if (returnAggregateFunctionColumn.substr(0, 3) == "CNT")
        parsedQuery.returnAggregateFunction = CNT;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.returnAggregateColumn = returnAggregateFunctionColumn.substr(4, returnAggregateFunctionColumn.size() - 5);

    return true;
}

bool semanticParseGROUPBY()
{
    logger.log("semanticParseGROUPBY");

    if (tableCatalogue.isTable(parsedQuery.groupByResultTableName))
    {
        cout << "SEMANTIC ERROR: Resultant table already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.groupByTableName))
    {
        cout << "SEMANTIC ERROR: Table doesn't exist" << endl;
        return false;
    }

    // More semantic checks to be added here
    Table *table = tableCatalogue.getTable(parsedQuery.groupByTableName);
    if (!table->isColumn(parsedQuery.groupingAttributeName))
    {
        cout << "SEMANTIC ERROR: Grouping attribute not present in table" << endl;
        return false;
    }

    if (!table->isColumn(parsedQuery.havingAggregateColumn))
    {
        cout << "SEMANTIC ERROR: Having aggregate column not present in table" << endl;
        return false;
    }

    if (!table->isColumn(parsedQuery.returnAggregateColumn))
    {
        cout << "SEMANTIC ERROR: Return aggregate column not present in table" << endl;
        return false;
    }

    if (!(parsedQuery.havingOperator == LESS_THAN || parsedQuery.havingOperator == GREATER_THAN || parsedQuery.havingOperator == LEQ || parsedQuery.havingOperator == GEQ || parsedQuery.havingOperator == EQUAL))
    {
        cout << "SEMANTIC ERROR: Invalid having operator" << endl;
        return false;
    }

    if (!(parsedQuery.havingAggregateFunction == MIN || parsedQuery.havingAggregateFunction == MAX || parsedQuery.havingAggregateFunction == SUM || parsedQuery.havingAggregateFunction == AVG || parsedQuery.havingAggregateFunction == CNT))
    {
        cout << "SEMANTIC ERROR: Invalid having aggregate function" << endl;
        return false;
    }

    if (!(parsedQuery.returnAggregateFunction == MIN || parsedQuery.returnAggregateFunction == MAX || parsedQuery.returnAggregateFunction == SUM || parsedQuery.returnAggregateFunction == AVG || parsedQuery.returnAggregateFunction == CNT))
    {
        cout << "SEMANTIC ERROR: Invalid return aggregate function" << endl;
        return false;
    }

    return true;
}

void executeGROUPBY()
{
    logger.log("executeGROUPBY");

    Table *oldTable = tableCatalogue.getTable(parsedQuery.groupByTableName);
    vector<string> columnName;
    columnName.push_back(parsedQuery.groupingAttributeName);
    vector<SortingStrategy> strtgy;
    strtgy.push_back(ASC);

    // Sort the tempTable according to the column name and sorting strategy provided
    string str = oldTable->sortTable(columnName, strtgy);
    
    Table *tempTable = new Table("TEMP" + oldTable->tableName, oldTable->columns, false);
    tempTable->distinctValuesPerColumnCount = oldTable->distinctValuesPerColumnCount;
    tempTable->blockCount = oldTable->blockCount;
    tempTable->rowCount = oldTable->rowCount;
    tempTable->rowsPerBlockCount = oldTable->rowsPerBlockCount;

    for (int i = 0; i < oldTable->blockCount; i++)
    {
        string pgOldName = "../data/temp/"+ str + "_Page" + to_string(i);
        string pgNewName = "../data/temp/"+ tempTable->tableName + "_Page" + to_string(i);
        rename(pgOldName.c_str(),pgNewName.c_str());
    }

    tableCatalogue.insertTable(tempTable);
  
    vector<string> columnNames;
    columnNames.push_back(parsedQuery.groupingAttributeName);
    columnNames.push_back(parsedQuery.returnAggregateFunctionColumn);

    map<int, double> groupHavingMap, groupReturnMap, countMap;

    Cursor cursor = tempTable->getCursor();
    vector<int> row = cursor.getNext();
    while (row.size() != 0)
    {
        int groupKey = row[tempTable->getColumnIndex(parsedQuery.groupingAttributeName)];
        countMap[groupKey] += 1;

        int aggregateValue = row[tempTable->getColumnIndex(parsedQuery.havingAggregateColumn)];
        int returnAggregateValue = row[tempTable->getColumnIndex(parsedQuery.returnAggregateColumn)];

        if (groupHavingMap.find(groupKey) == groupHavingMap.end())
        {
            cout << aggregateValue << endl;
            groupHavingMap[groupKey] = aggregateValue;
        }
        else
        {
            switch (parsedQuery.havingAggregateFunction)
            {
            case MIN:
                groupHavingMap[groupKey] = min(groupHavingMap[groupKey], (double)aggregateValue);
                break;
            case MAX:
                groupHavingMap[groupKey] = max(groupHavingMap[groupKey], (double)aggregateValue);
                break;
            case SUM:
                groupHavingMap[groupKey] += aggregateValue;
                break;
            case AVG:
                groupHavingMap[groupKey] += aggregateValue;
                break;
            case CNT:
                groupHavingMap[groupKey] += 1;
                break;
            }
        }

        if (groupReturnMap.find(groupKey) == groupReturnMap.end())
        {
            groupReturnMap[groupKey] = returnAggregateValue;
        }
        else
        {
            switch (parsedQuery.returnAggregateFunction)
            {
            case MIN:
                groupReturnMap[groupKey] = min(groupReturnMap[groupKey], (double)returnAggregateValue);
                break;
            case MAX:
                groupReturnMap[groupKey] = max(groupReturnMap[groupKey], (double)returnAggregateValue);
                break;
            case SUM:
                groupReturnMap[groupKey] += returnAggregateValue;
                break;
            case AVG:
                groupReturnMap[groupKey] += returnAggregateValue;
                break;
            case CNT:
                groupReturnMap[groupKey] += 1;
                break;
            }
        }
        row = cursor.getNext();
    }
    // Delete the temporary files
    for (int i = 0; i < tempTable->blockCount; i++)
    {
        string pgName = "../data/temp/"+ tempTable->tableName + "_Page" + to_string(i);
        remove(pgName.c_str());
    }

    for (auto group : groupHavingMap)
    {
        int groupKey = group.first;
        int havingAggregateValue = group.second;
        int returnAggregateValue = groupReturnMap[groupKey];

        switch (parsedQuery.havingAggregateFunction)
        {
        case AVG:
            groupHavingMap[groupKey] /= (double)countMap[groupKey];
            break;
        }

        switch (parsedQuery.returnAggregateFunction)
        {
        case AVG:
            groupReturnMap[groupKey] /= (double)countMap[groupKey];
            break;
        }
    }
    
    string fileName = "../data/" + parsedQuery.groupByResultTableName + ".csv";
    ofstream fout(fileName);
    fout << parsedQuery.groupingAttributeName << "," << parsedQuery.returnAggregateFunctionColumn << endl;

    for (auto group : groupHavingMap)
    {
        int groupKey = group.first;
        int havingAggregateValue = group.second;
        int returnAggregateValue = groupReturnMap[groupKey];

        if ((parsedQuery.havingOperator == LESS_THAN && havingAggregateValue < (double)stoi(parsedQuery.havingValue))
        || (parsedQuery.havingOperator == GREATER_THAN && havingAggregateValue > (double)stoi(parsedQuery.havingValue))
        || (parsedQuery.havingOperator == LEQ && havingAggregateValue <= (double)stoi(parsedQuery.havingValue))
        || (parsedQuery.havingOperator == GEQ && havingAggregateValue >= (double)stoi(parsedQuery.havingValue))
        )
        {
            fout << groupKey << "," << returnAggregateValue << endl;
        }
    }
    fout.close();

    // From temp folder remove the TEMP + tableName.csv file
    string tempFileName = "../data/temp/" + tempTable->tableName + ".csv";
    remove(tempFileName.c_str());
    // From temp folder remove the groupByResultTableName.csv file
    tempFileName = "../data/temp/" + parsedQuery.groupByResultTableName + ".csv";
    remove(tempFileName.c_str());

    Table *table = new Table(parsedQuery.groupByResultTableName, false);
    if (table->load())
    {
        tableCatalogue.insertTable(table);
        cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    }
    return;
    // T2 <- GROUP BY Sex FROM EMPLOYEE HAVING MAX(Salary) > 1000 RETURN MIN(Salary)
}
