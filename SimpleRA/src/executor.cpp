#include "global.h"

void executeCommand()
{

    switch (parsedQuery.queryType)
    {
    case CLEAR:
        executeCLEAR();
        break;
    case CROSS:
        executeCROSS();
        break;
    case DISTINCT:
        executeDISTINCT();
        break;
    case EXPORT:
        executeEXPORT();
        break;
    case INDEX:
        executeINDEX();
        break;
    case JOIN:
        executeJOIN();
        break;
    case ORDER_BY:
        executeORDERBY();
        break;
    case GROUP_BY:
        executeGROUPBY();
        break;
    case LIST:
        executeLIST();
        break;
    case LOAD:
        executeLOAD();
        break;
    case PRINT:
        executePRINT();
        break;
    case PROJECTION:
        executePROJECTION();
        break;
    case RENAME:
        executeRENAME();
        break;
    case SELECTION:
        executeSELECTION();
        break;
    case SORT:
        executeSORT();
        break;
    case SOURCE:
        executeSOURCE();
        break;
    case LOAD_MATRIX:
        executeLOAD_MATRIX();
        break;
    case PRINT_MATRIX:
        executePRINT_MATRIX();
        break;
    case EXPORT_MATRIX:
        executeEXPORT_MATRIX();
        break;
    case RENAME_MATRIX:
        executeRENAME_MATRIX();
        break;
    case TRANSPOSE_MATRIX:
        executeTRANSPOSE_MATRIX();
        break;
    case CHECKSYMMETRY:
        executeCHECKSYMMETRY();
        break;
    case COMPUTE:
        executeCOMPUTE();
        break;
    default:
        cout << "PARSING ERROR" << endl;
    }

    return;
}

void printRowCount(int rowCount)
{
    cout << "\n\nRow Count: " << rowCount << endl;
    return;
}