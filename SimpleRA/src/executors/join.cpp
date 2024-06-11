#include "global.h"
/**
 * @brief
 * SYNTAX: R <- JOIN relation_name1, relation_name2 ON column_name1 bin_op column_name2
 */
bool syntacticParseJOIN()
{
    logger.log("syntacticParseJOIN");
    if (tokenizedQuery.size() != 9 || tokenizedQuery[5] != "ON")
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = JOIN;
    parsedQuery.joinResultRelationName = tokenizedQuery[0];
    parsedQuery.joinFirstRelationName = tokenizedQuery[3];
    parsedQuery.joinSecondRelationName = tokenizedQuery[4];
    parsedQuery.joinFirstColumnName = tokenizedQuery[6];
    parsedQuery.joinSecondColumnName = tokenizedQuery[8];

    string binaryOperator = tokenizedQuery[7];
    if (binaryOperator == "<")
        parsedQuery.joinBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.joinBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.joinBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.joinBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.joinBinaryOperator = EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseJOIN()
{
    logger.log("semanticParseJOIN");

    if (tableCatalogue.isTable(parsedQuery.joinResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.joinFirstRelationName) || !tableCatalogue.isTable(parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    Table *table1 = tableCatalogue.getTable(parsedQuery.joinFirstRelationName);
    Table *table2 = tableCatalogue.getTable(parsedQuery.joinSecondRelationName);

    if (!table1->isColumn(parsedQuery.joinFirstColumnName) || !table2->isColumn(parsedQuery.joinSecondColumnName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    return true;
}

void handleLESS_THAN(Table *t1, Table *t2, ofstream &fout)
{
    logger.log("handleLESS_THAN");

    Cursor cursor1 = t1->getCursor();
    vector<int> row1 = cursor1.getNext();
    while (row1.size() != 0)
    {
        Cursor cursor2 = t2->getCursor();
        vector<int> row2 = cursor2.getNext();
        while (row2.size() != 0)
        {
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] >= row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                row2 = cursor2.getNext();
                continue;
            }
            // Insert all rows where the row1 < row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] < row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                vector<int> row;
                for (auto element : row1)
                    row.push_back(element);
                for (auto element : row2)
                    row.push_back(element);

                for (int i = 0; i < row.size(); i++)
                {
                    fout << row[i];
                    if (i != row.size() - 1)
                        fout << ",";
                }
                fout << endl;

                row2 = cursor2.getNext();
                continue;
            }
        }
        row1 = cursor1.getNext();
    }
}
void handleGREATER_THAN(Table *t1, Table *t2, ofstream &fout)
{
    logger.log("handleGREATER_THAN");

    Cursor cursor1 = t1->getCursor();
    vector<int> row1 = cursor1.getNext();
    while (row1.size() != 0)
    {
        Cursor cursor2 = t2->getCursor();
        vector<int> row2 = cursor2.getNext();
        while (row2.size() != 0)
        {
            // Insert all rows where the row1 > row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] > row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                vector<int> row;
                for (auto element : row1)
                    row.push_back(element);
                for (auto element : row2)
                    row.push_back(element);

                for (int i = 0; i < row.size(); i++)
                {
                    fout << row[i];
                    if (i != row.size() - 1)
                        fout << ",";
                }
                fout << endl;

                row2 = cursor2.getNext();
                continue;
            }
            break;
        }
        row1 = cursor1.getNext();
    }
}
void handleGEQ(Table *t1, Table *t2, ofstream &fout)
{
    logger.log("handleGEQ");

    Cursor cursor1 = t1->getCursor();
    vector<int> row1 = cursor1.getNext();
    while (row1.size() != 0)
    {
        Cursor cursor2 = t2->getCursor();
        vector<int> row2 = cursor2.getNext();
        while (row2.size() != 0)
        {
            // Insert all rows where the row1 == row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] >= row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                vector<int> row;
                for (auto element : row1)
                    row.push_back(element);
                for (auto element : row2)
                    row.push_back(element);

                for (int i = 0; i < row.size(); i++)
                {
                    fout << row[i];
                    if (i != row.size() - 1)
                        fout << ",";
                }
                fout << endl;

                row2 = cursor2.getNext();
                continue;
            }
            // Ignore all rows where the row1 < row2
            break;
        }
        row1 = cursor1.getNext();
    }
}
void handleLEQ(Table *t1, Table *t2, ofstream &fout)
{
    logger.log("handleLEQ");

    Cursor cursor1 = t1->getCursor();
    vector<int> row1 = cursor1.getNext();
    while (row1.size() != 0)
    {
        Cursor cursor2 = t2->getCursor();
        vector<int> row2 = cursor2.getNext();
        while (row2.size() != 0)
        {
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] > row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                row2 = cursor2.getNext();
                continue;
            }
            // Insert all rows where the row1 <= row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] <= row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                vector<int> row;
                for (auto element : row1)
                    row.push_back(element);
                for (auto element : row2)
                    row.push_back(element);
                
                for (int i = 0; i < row.size(); i++)
                {
                    fout << row[i];
                    if (i != row.size() - 1)
                        fout << ",";
                }
                fout << endl;

                row2 = cursor2.getNext();
                continue;
            }
        }
        row1 = cursor1.getNext();
    }
}
void handleEQUAL(Table *t1, Table *t2, ofstream &fout)
{
    logger.log("handleEQUAL");

    Cursor cursor1 = t1->getCursor();
    vector<int> row1 = cursor1.getNext();
    while (row1.size() != 0)
    {
        Cursor cursor2 = t2->getCursor();
        vector<int> row2 = cursor2.getNext();
        while (row2.size() != 0)
        {
            // Ignore all rows where the row1 < row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] > row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                row2 = cursor2.getNext();
                continue;
            }
            // Insert all rows where the row1 == row2
            while (row2.size() != 0 && row1[t1->getColumnIndex(parsedQuery.joinFirstColumnName)] == row2[t2->getColumnIndex(parsedQuery.joinSecondColumnName)])
            {
                vector<int> row;
                for (auto element : row1)
                    row.push_back(element);
                for (auto element : row2)
                    row.push_back(element);
                
                for (int i = 0; i < row.size(); i++)
                {
                    fout << row[i];
                    if (i != row.size() - 1)
                        fout << ",";
                }
                fout << endl;

                row2 = cursor2.getNext();
                continue;
            }
            // Ignore all rows where the row1 > row2
            break;
        }
        row1 = cursor1.getNext();
    }
}
void executeJOIN()
{
    logger.log("executeJOIN");

    // Sort the two tables
    Table *oldTable1 = tableCatalogue.getTable(parsedQuery.joinFirstRelationName);
    vector<SortingStrategy> strtgy1;    
    strtgy1.push_back(ASC);
    vector<string> columnName1;
    columnName1.push_back(parsedQuery.joinFirstColumnName);
 
    Table *t1 = new Table("TEMP" + parsedQuery.joinFirstRelationName, oldTable1->columns, false);
    t1->distinctValuesPerColumnCount = oldTable1->distinctValuesPerColumnCount;
    t1->blockCount = oldTable1->blockCount;
    t1->rowCount = oldTable1->rowCount;
    t1->rowsPerBlockCount = oldTable1->rowsPerBlockCount;

    // Sort the table according to the column name and sorting strategy provided
    string str1 = oldTable1->sortTable(columnName1, strtgy1);
    for (int i = 0; i < oldTable1->blockCount; i++)
    {
        string pgOldName = "../data/temp/"+ str1 + "_Page" + to_string(i);
        string pgNewName = "../data/temp/"+ t1->tableName + "_Page" + to_string(i);
        rename(pgOldName.c_str(),pgNewName.c_str());
    }
    tableCatalogue.insertTable(t1);

    Table *oldTable2 = tableCatalogue.getTable(parsedQuery.joinSecondRelationName);
    vector<SortingStrategy> strtgy2;    
    strtgy2.push_back(ASC);
    vector<string> columnName2;
    columnName2.push_back(parsedQuery.joinSecondColumnName);
 
    Table *t2 = new Table("TEMP" + parsedQuery.joinSecondRelationName, oldTable2->columns, false);
    t2->distinctValuesPerColumnCount = oldTable2->distinctValuesPerColumnCount;
    t2->blockCount = oldTable2->blockCount;
    t2->rowCount = oldTable2->rowCount;
    t2->rowsPerBlockCount = oldTable2->rowsPerBlockCount;

    // Sort the table according to the column name and sorting strategy provided
    string str2 = oldTable2->sortTable(columnName2, strtgy2);
    for (int i = 0; i < oldTable2->blockCount; i++)
    {
        string pgOldName = "../data/temp/"+ str2 + "_Page" + to_string(i);
        string pgNewName = "../data/temp/"+ t2->tableName + "_Page" + to_string(i);
        rename(pgOldName.c_str(),pgNewName.c_str());
    }
    tableCatalogue.insertTable(t2);

    vector<string> columnNames;
    for (auto columnName : t1->columns)
        columnNames.push_back(columnName);
    for (auto columnName : t2->columns)
        columnNames.push_back(columnName);


    string fileName = "../data/" + parsedQuery.joinResultRelationName + ".csv";
    ofstream fout(fileName);
    for (int i = 0; i < columnNames.size(); i++)
    {
        fout << columnNames[i];
        if (i != columnNames.size() - 1)
            fout << ",";
    }
    fout << endl;

    switch (parsedQuery.joinBinaryOperator)
    {
    case LESS_THAN:
        handleLESS_THAN(t1, t2, fout);
        break;
    case GREATER_THAN:
        handleGREATER_THAN(t1, t2, fout);
        break;
    case GEQ:
        handleGEQ(t1, t2, fout);
        break;
    case LEQ:
        handleLEQ(t1, t2, fout);
        break;
    case EQUAL:
        handleEQUAL(t1, t2, fout);
        break;
    default:
        break;
    }
    fout.close();

    // From the temp folder, remove the temporary files
    for (int i = 0; i < t1->blockCount; i++)
    {
        string pgName = "../data/temp/"+ t1->tableName + "_Page" + to_string(i);
        remove(pgName.c_str());
    }
    for (int i = 0; i < t2->blockCount; i++)
    {
        string pgName = "../data/temp/"+ t2->tableName + "_Page" + to_string(i);
        remove(pgName.c_str());
    }
    string tempFileName1 = "../data/temp/" + t1->tableName + ".csv";
    string tempFileName2 = "../data/temp/" + t2->tableName + ".csv";
    remove(tempFileName1.c_str());
    remove(tempFileName2.c_str());
    
    Table *resultantTable = new Table(parsedQuery.joinResultRelationName, false);
    if (resultantTable->load())
    {
        tableCatalogue.insertTable(resultantTable);
        cout << "Result of join successfully stored in table " << parsedQuery.joinResultRelationName << endl;
    }
    return;
}