#include "global.h"

bool syntacticParse()
{
    logger.log("syntacticParse");
    string possibleQueryType = tokenizedQuery[0];

    if (tokenizedQuery.size() < 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    if (possibleQueryType == "CLEAR")
        return syntacticParseCLEAR();
    else if (possibleQueryType == "INDEX")
        return syntacticParseINDEX();
    else if (possibleQueryType == "LIST")
        return syntacticParseLIST();
    else if (possibleQueryType == "LOAD")
    {
        if (tokenizedQuery.size() == 2)
            return syntacticParseLOAD();
        else if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX")
            return syntacticParseLOAD_MATRIX();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    else if (possibleQueryType == "PRINT")
    {
        if (tokenizedQuery.size() == 2)
            return syntacticParsePRINT();
        else if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX")
            return syntacticParsePRINT_MATRIX();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    else if (possibleQueryType == "RENAME")
    {
        if (tokenizedQuery.size() == 4 && tokenizedQuery[1] == "MATRIX")
            return syntacticParseRENAME_MATRIX();
        else if (tokenizedQuery.size() == 4)
            return syntacticParseRENAME();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    else if (possibleQueryType == "EXPORT")
    {
        if (tokenizedQuery.size() == 2)
            return syntacticParseEXPORT();
        else if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX")
            return syntacticParseEXPORT_MATRIX();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    else if (possibleQueryType == "TRANSPOSE" && tokenizedQuery[1] == "MATRIX")
        return syntacticParseTRANSPOSE_MATRIX();
    else if (possibleQueryType == "CHECKSYMMETRY")
        return syntacticParseCHECKSYMMETRY();
    else if (possibleQueryType == "COMPUTE")
        return syntacticParseCOMPUTE();
    else if (possibleQueryType == "SOURCE")
        return syntacticParseSOURCE();
    else if (possibleQueryType == "SORT")
        return syntacticParseSORT();
    else
    {
        string resultantRelationName = possibleQueryType;
        if (tokenizedQuery[1] != "<-" || tokenizedQuery.size() < 3)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        possibleQueryType = tokenizedQuery[2];
        if (possibleQueryType == "PROJECT")
            return syntacticParsePROJECTION();
        else if (possibleQueryType == "SELECT")
            return syntacticParseSELECTION();
        else if (possibleQueryType == "JOIN")
            return syntacticParseJOIN();
        else if (possibleQueryType == "GROUP")
            return syntacticParseGROUPBY();
        else if (possibleQueryType == "ORDER")
            return syntacticParseORDERBY();
        else if (possibleQueryType == "CROSS")
            return syntacticParseCROSS();
        else if (possibleQueryType == "DISTINCT")
            return syntacticParseDISTINCT();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    return false;
}

ParsedQuery::ParsedQuery()
{
}

void ParsedQuery::clear()
{
    logger.log("ParseQuery::clear");
    this->queryType = UNDETERMINED;

    this->clearRelationName = "";

    this->crossResultRelationName = "";
    this->crossFirstRelationName = "";
    this->crossSecondRelationName = "";

    this->distinctResultRelationName = "";
    this->distinctRelationName = "";

    this->exportRelationName = "";

    this->indexingStrategy = NOTHING;
    this->indexColumnName = "";
    this->indexRelationName = "";

    this->joinBinaryOperator = NO_BINOP_CLAUSE;
    this->joinResultRelationName = "";
    this->joinFirstRelationName = "";
    this->joinSecondRelationName = "";
    this->joinFirstColumnName = "";
    this->joinSecondColumnName = "";

    this->loadRelationName = "";

    this->printRelationName = "";

    this->projectionResultRelationName = "";
    this->projectionColumnList.clear();
    this->projectionRelationName = "";

    this->renameFromColumnName = "";
    this->renameToColumnName = "";
    this->renameRelationName = "";

    this->selectType = NO_SELECT_CLAUSE;
    this->selectionBinaryOperator = NO_BINOP_CLAUSE;
    this->selectionResultRelationName = "";
    this->selectionRelationName = "";
    this->selectionFirstColumnName = "";
    this->selectionSecondColumnName = "";
    this->selectionIntLiteral = 0;

    this->sortingStrategy = NO_SORT_CLAUSE;
    this->sortResultRelationName = "";
    this->sortColumnName = "";
    this->sortRelationName = "";

    this->sourceFileName = "";

    this->renameMatrixOldName = "";
    this->renameMatrixNewName = "";

    this->transposeMatrixName = "";
    this->checkSymmetricMatrixName = "";
    this->computeMatrixName = "";
}

/**
 * @brief Checks to see if source file exists. Called when LOAD command is
 * invoked.
 *
 * @param tableName
 * @return true
 * @return false
 */
bool isFileExists(string tableName)
{
    string fileName = "../data/" + tableName + ".csv";
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

/**
 * @brief Checks to see if source file exists. Called when SOURCE command is
 * invoked.
 *
 * @param tableName
 * @return true
 * @return false
 */
bool isQueryFile(string fileName)
{
    fileName = "../data/" + fileName + ".ra";
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}
