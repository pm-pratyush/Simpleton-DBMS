#include "tableCatalogue.h"

using namespace std;

#ifndef SYNTACTICPARSER_H
#define SYNTACTICPARSER_H

enum QueryType
{
    CLEAR,
    CROSS,
    DISTINCT,
    EXPORT,
    INDEX,
    JOIN,
    GROUP_BY,
    ORDER_BY,
    LIST,
    LOAD,
    PRINT,
    PROJECTION,
    RENAME,
    SELECTION,
    SORT,
    SOURCE,

    // Project Phase: 1
    LOAD_MATRIX,
    PRINT_MATRIX,
    EXPORT_MATRIX,
    RENAME_MATRIX,
    TRANSPOSE_MATRIX,
    CHECKSYMMETRY,
    COMPUTE,

    UNDETERMINED
};

enum BinaryOperator
{
    LESS_THAN,
    GREATER_THAN,
    LEQ,
    GEQ,
    EQUAL,
    NOT_EQUAL,
    NO_BINOP_CLAUSE
};

enum AggregateFunction
{
    MIN,
    MAX,
    SUM,
    AVG,
    CNT,
    NO_FUNC_CLAUSE
};

enum SelectType
{
    COLUMN,
    INT_LITERAL,
    NO_SELECT_CLAUSE
};

class ParsedQuery
{

public:
    QueryType queryType = UNDETERMINED;

    string clearRelationName = "";

    string crossResultRelationName = "";
    string crossFirstRelationName = "";
    string crossSecondRelationName = "";

    string distinctResultRelationName = "";
    string distinctRelationName = "";

    string exportRelationName = "";

    IndexingStrategy indexingStrategy = NOTHING;
    string indexColumnName = "";
    string indexRelationName = "";

    BinaryOperator joinBinaryOperator = NO_BINOP_CLAUSE;
    string joinResultRelationName = "";
    string joinFirstRelationName = "";
    string joinSecondRelationName = "";
    string joinFirstColumnName = "";
    string joinSecondColumnName = "";

    SortingStrategy orderBySortingStrategy = NO_SORT_CLAUSE;
    string orderByResultTableName = "";
    string orderByColumnName = "";
    string orderByTableName = "";

    BinaryOperator havingOperator = NO_BINOP_CLAUSE;
    string groupByResultTableName = "";
    string groupingAttributeName = "";
    string groupByTableName = "";
    string havingValue = "";
    
    string havingAggregateFunctionColumn = "";
    string returnAggregateFunctionColumn = "";

    string havingAggregateColumn = "";
    string returnAggregateColumn = "";
    AggregateFunction returnAggregateFunction = NO_FUNC_CLAUSE;
    AggregateFunction havingAggregateFunction = NO_FUNC_CLAUSE;

    string loadRelationName = "";

    string printRelationName = "";

    string projectionResultRelationName = "";
    vector<string> projectionColumnList;
    string projectionRelationName = "";

    string renameFromColumnName = "";
    string renameToColumnName = "";
    string renameRelationName = "";

    SelectType selectType = NO_SELECT_CLAUSE;
    BinaryOperator selectionBinaryOperator = NO_BINOP_CLAUSE;
    string selectionResultRelationName = "";
    string selectionRelationName = "";
    string selectionFirstColumnName = "";
    string selectionSecondColumnName = "";
    int selectionIntLiteral = 0;

    SortingStrategy sortingStrategy = NO_SORT_CLAUSE;
    string sortResultRelationName = "";
    string sortColumnName = "";
    string sortRelationName = "";

    string sourceFileName = "";

    string renameMatrixOldName = "";
    string renameMatrixNewName = "";

    string transposeMatrixName = "";

    string checkSymmetricMatrixName = "";
    string computeMatrixName = "";

    // Project Phase 2:
    vector<string> sortAttributeNames;
    vector<SortingStrategy> sortSortingStrategies;

    ParsedQuery();
    void clear();
};

bool syntacticParse();
bool syntacticParseCLEAR();
bool syntacticParseCROSS();
bool syntacticParseDISTINCT();
bool syntacticParseEXPORT();
bool syntacticParseINDEX();
bool syntacticParseLIST();
bool syntacticParseLOAD();
bool syntacticParsePRINT();
bool syntacticParsePROJECTION();
bool syntacticParseRENAME();
bool syntacticParseSELECTION();
bool syntacticParseSORT();
bool syntacticParseSOURCE();

// Project Phase: 1
bool syntacticParseLOAD_MATRIX();
bool syntacticParsePRINT_MATRIX();
bool syntacticParseEXPORT_MATRIX();
bool syntacticParseRENAME_MATRIX();
bool syntacticParseTRANSPOSE_MATRIX();
bool syntacticParseCHECKSYMMETRY();
bool syntacticParseCOMPUTE();

// Project Phase 2:
bool syntacticParseGROUPBY();
bool syntacticParseORDERBY();
bool syntacticParseJOIN();

bool isFileExists(string tableName);
bool isQueryFile(string fileName);

#endif