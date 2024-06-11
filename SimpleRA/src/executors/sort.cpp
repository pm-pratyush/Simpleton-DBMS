#include "global.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/**
 * @brief File contains method to process SORT commands.
 *
 * syntax:
 * SORT relation_name BY <column_name1, column_name2,..., column_namek> IN <ASC|DESC, ASC|DESC,..., ASC|DESC>
 *
 */
bool syntacticParseSORT()
{
    logger.log("syntacticParseSORT");

    // SORT <table_name> BY <column_name1, column_name2,..., column_namek> IN <ASC|DESC, ASC|DESC,..., ASC|DESC>
    parsedQuery.queryType = SORT;
    parsedQuery.sortRelationName = tokenizedQuery[1];

    if (tokenizedQuery[2] != "BY")
    {
        cout << "SYNTAX ERROR: \"BY\" NOT PRESENT" << endl;
        return false;
    }
    if (tokenizedQuery.size() <= 5)
    {
        cout << "SYNTAX ERROR: INVALID NUMBER OF ARGUMENTS" << endl;
        return false;
    }

    int IN_index = -1;
    parsedQuery.sortAttributeNames.clear();
    parsedQuery.sortSortingStrategies.clear();
    for (int i = 3; i < tokenizedQuery.size(); i++)
    {
        if (tokenizedQuery[i] != "IN")
        {
            if (IN_index == -1)
            {
                parsedQuery.sortAttributeNames.push_back(tokenizedQuery[i]);
            }
            else
            {
                if (tokenizedQuery[i] == "ASC")
                {
                    parsedQuery.sortSortingStrategies.push_back(ASC);
                }
                else if (tokenizedQuery[i] == "DESC")
                {
                    parsedQuery.sortSortingStrategies.push_back(DESC);
                }
                else
                {
                    cout << "SYNTAX ERROR: SORTING STRATEGY MUST BE ASC OR DESC" << endl;
                    return false;
                }
            }
        }
        else
        {
            IN_index = i;
        }
    }

    if (IN_index == -1)
    {
        cout << "SYNTAX ERROR: \"IN\" NOT PRESENT" << endl;
        return false;
    }

    if (parsedQuery.sortAttributeNames.size() != parsedQuery.sortSortingStrategies.size())
    {
        cout << "SYNTAX ERROR: NUMBER OF SORTING STRATEGIES AND NUMBER OF ATTRIBUTE NAMES MUST BE SAME" << endl;
        return false;
    }

    return true;
}

bool semanticParseSORT()
{
    logger.log("semanticParseSORT");

    if (!tableCatalogue.isTable(parsedQuery.sortRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    for (int i = 0; i < parsedQuery.sortAttributeNames.size(); i++)
    {
        if (!tableCatalogue.isColumnFromTable(parsedQuery.sortAttributeNames[i], parsedQuery.sortRelationName))
        {
            cout << "SEMANTIC ERROR: Column " + parsedQuery.sortAttributeNames[i] + " doesn't exist in relation" << endl;
            return false;
        }
    }

    return true;
}

void executeSORT()
{
    Table *tablePtr = tableCatalogue.getTable(parsedQuery.sortRelationName);
    bufferManager.clearPool();

    string str = tablePtr->sortTable(parsedQuery.sortAttributeNames, parsedQuery.sortSortingStrategies);

    for (int i = 0; i < tablePtr->blockCount; i++)
    {
        bufferManager.deleteFile(tablePtr->tableName, i);
        string pgOldName = "../data/temp/"+ str + "_Page" + to_string(i);
        string pgNewName = "../data/temp/"+ tablePtr->tableName + "_Page" + to_string(i);
        rename(pgOldName.c_str(),pgNewName.c_str());
    }
    tablePtr->makePermanent();
}