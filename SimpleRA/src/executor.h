#include "semanticParser.h"

#ifndef EXECUTOR_H
#define EXECUTOR_H

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeINDEX();
void executeLIST();
void executeLOAD();
void executePRINT();
void executePROJECTION();
void executeRENAME();
void executeSELECTION();
void executeSORT();
void executeSOURCE();

// Project Phase: 1
void executeLOAD_MATRIX();
void executePRINT_MATRIX();
void executeEXPORT_MATRIX();
void executeRENAME_MATRIX();
void executeTRANSPOSE_MATRIX();
void executeCHECKSYMMETRY();
void executeCOMPUTE();

// Project Phase: 2
void executeGROUPBY();
void executeORDERBY();
void executeJOIN();

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);

#endif