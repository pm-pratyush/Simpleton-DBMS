#include "table.h"

#ifndef TABLECATALOGUE_H
#define TABLECATALOGUE_H

/**
 * @brief The TableCatalogue acts like an index of tables existing in the
 * system. Everytime a table is added(removed) to(from) the system, it needs to
 * be added(removed) to(from) the tableCatalogue.
 *
 */
class TableCatalogue
{

    unordered_map<string, Table *> tables;

public:
    TableCatalogue() {}
    void insertTable(Table *table);
    void deleteTable(string tableName);
    void renameTable(string fromTableName, string toTableName);
    Table *getTable(string tableName);
    bool isTable(string tableName);
    bool isColumnFromTable(string columnName, string tableName);
    void print();
    ~TableCatalogue();
};

#endif