#include <iostream>
using namespace std;

#include "executor.h"

extern float BLOCK_SIZE;
extern int NO_OF_BLOCK_READ;
extern int NO_OF_BLOCK_WRITTEN;
extern int NO_OF_BLOCK_ACCESSED;
extern uint BLOCK_COUNT;
extern uint PRINT_COUNT;
extern vector<string> tokenizedQuery;
extern ParsedQuery parsedQuery;
extern TableCatalogue tableCatalogue;
extern BufferManager bufferManager;