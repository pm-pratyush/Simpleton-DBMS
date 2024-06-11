#include "syntacticParser.h"

#ifndef SEMANTICPARSER_H
#define SEMANTICPARSER_H

bool semanticParse();

bool semanticParseCLEAR();
bool semanticParseCROSS();
bool semanticParseDISTINCT();
bool semanticParseEXPORT();
bool semanticParseINDEX();
bool semanticParseLIST();
bool semanticParseLOAD();
bool semanticParsePRINT();
bool semanticParsePROJECTION();
bool semanticParseRENAME();
bool semanticParseSELECTION();
bool semanticParseSORT();
bool semanticParseSOURCE();

// Project Phase: 1
bool semanticParseLOAD_MATRIX();
bool semanticParsePRINT_MATRIX();
bool semanticParseEXPORT_MATRIX();
bool semanticParseRENAME_MATRIX();
bool semanticParseTRANSPOSE_MATRIX();
bool semanticParseCHECKSYMMETRY();
bool semanticParseCOMPUTE();

// Project Phase: 2
bool semanticParseGROUPBY();
bool semanticParseORDERBY();
bool semanticParseJOIN();

#endif