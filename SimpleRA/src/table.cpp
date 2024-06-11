#include "global.h"
#include <cmath>

string srtTbName;
vector<string> srtAttr; 
vector<SortingStrategy> srtStrtgy;


/**
 * @brief Construct a new Table:: Table object
 *
 */
Table::Table()
{
    logger.log("Table::Table");
}

/**
 * @brief Construct a new Table:: Table object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param tableName
 */
Table::Table(string tableName, bool isMatrix)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/" + tableName + ".csv";
    this->tableName = tableName;
    this->isMatrix = isMatrix;
}

/**
 * @brief Construct a new Table:: Table object used when an assignment command
 * is encountered. To create the table object both the table name and the
 * columns the table holds should be specified.
 *
 * @param tableName
 * @param columns
 */
Table::Table(string tableName, vector<string> columns, bool isMatrix)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/temp/" + tableName + ".csv";
    this->tableName = tableName;
    this->columns = columns;
    this->columnCount = columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * columnCount));
    this->writeRow<string>(columns);
    this->isMatrix = isMatrix;
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates table
 * statistics.
 *
 * @return true if the table has been successfully loaded
 * @return false if an error occurred
 */
bool Table::load()
{
    logger.log("Table::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        if (this->extractColumnNames(line))
            if (this->blockify())
                return true;
    }
    fin.close();
    return false;
}

/**
 * @brief Function extracts column names from the header line of the .csv data
 * file.
 *
 * @param line
 * @return true if column names successfully extracted (i.e. no column name
 * repeats)
 * @return false otherwise
 */
bool Table::extractColumnNames(string firstLine)
{
    logger.log("Table::extractColumnNames");

    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ','))
    {
        if (this->isMatrix == false)
        {
            word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
            if (columnNames.count(word))
                return false;
            columnNames.insert(word);
        }
        this->columns.emplace_back(word);
    }
    this->columnCount = this->columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * this->columnCount));
    return true;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size.
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Table::blockify()
{
    logger.log("Table::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->columnCount, 0);
    vector<vector<int>> rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;
    unordered_set<int> dummy;
    dummy.clear();
    this->distinctValuesInColumns.assign(this->columnCount, dummy);
    this->distinctValuesPerColumnCount.assign(this->columnCount, 0);

    // Skip first line if not a matrix
    if (this->isMatrix == false)
        getline(fin, line);

    while (getline(fin, line))
    {
        stringstream s(line);
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (!getline(s, word, ','))
                return false;
            row[columnCounter] = stoi(word);
            rowsInPage[pageCounter][columnCounter] = row[columnCounter];
        }
        pageCounter++;
        this->updateStatistics(row);
        if (pageCounter == this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
            NO_OF_BLOCK_WRITTEN++;
        }
    }
    if (pageCounter)
    {
        bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
        NO_OF_BLOCK_WRITTEN++;
    }

    if (this->rowCount == 0)
        return false;
    this->distinctValuesInColumns.clear();
    return true;
}

/**
 * @brief Given a row of values, this function will update the statistics it
 * stores i.e. it updates the number of rows that are present in the column and
 * the number of distinct values present in each column. These statistics are to
 * be used during optimisation.
 *
 * @param row
 */
void Table::updateStatistics(vector<int> row)
{
    this->rowCount++;
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!this->distinctValuesInColumns[columnCounter].count(row[columnCounter]))
        {
            this->distinctValuesInColumns[columnCounter].insert(row[columnCounter]);
            this->distinctValuesPerColumnCount[columnCounter]++;
        }
    }
}

/**
 * @brief Checks if the given column is present in this table.
 *
 * @param columnName
 * @return true
 * @return false
 */
bool Table::isColumn(string columnName)
{
    logger.log("Table::isColumn");
    for (auto col : this->columns)
    {
        if (col == columnName)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Renames the column indicated by fromColumnName to toColumnName. It is
 * assumed that checks such as the existence of fromColumnName and the non prior
 * existence of toColumnName are done.
 *
 * @param fromColumnName
 * @param toColumnName
 */
void Table::renameColumn(string fromColumnName, string toColumnName)
{
    logger.log("Table::renameColumn");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (columns[columnCounter] == fromColumnName)
        {
            columns[columnCounter] = toColumnName;
            break;
        }
    }
    return;
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Table::print()
{
    logger.log("Table::print");
    uint count = min((long long)PRINT_COUNT, this->rowCount);

    // Print Headings if not a matrix
    if (this->isMatrix == false)
        this->writeRow(this->columns, cout);

    Cursor cursor(this->tableName, 0);
    vector<int> row;

    for (int rowCounter = 0; rowCounter < min((uint)20, count); rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, cout);
    }

    if (this->isMatrix == false)
        printRowCount(this->rowCount);
    else
        cout << "\nDimensions: " << this->rowCount << " * " << this->columnCount << endl;
    
    NO_OF_BLOCK_READ += (min((long long)PRINT_COUNT, this->rowCount) / this->maxRowsPerBlock);
    NO_OF_BLOCK_READ++;
}

/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor
 * @return vector<int>
 */
void Table::getNextPage(Cursor *cursor)
{
    logger.log("Table::getNext");

    if (cursor->pageIndex < this->blockCount - 1)
    {
        cursor->nextPage(cursor->pageIndex + 1);
    }

    NO_OF_BLOCK_READ++;
}

/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Table::makePermanent()
{
    logger.log("Table::makePermanent");

    bufferManager.clearPool();

    if (!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
        
    string newSourceFile = "../data/" + this->tableName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    // Print Headings if not a matrix
    if (this->isMatrix == false)
        this->writeRow(this->columns, fout);

    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, fout);
    }
    fout.close();

    NO_OF_BLOCK_WRITTEN += (this->rowCount / this->maxRowsPerBlock);
    NO_OF_BLOCK_WRITTEN++;
}

/**
 * @brief Function to check if table is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Table::isPermanent()
{
    logger.log("Table::isPermanent");
    if (this->sourceFileName == "../data/" + this->tableName + ".csv")
        return true;
    return false;
}

/**
 * @brief The unload function removes the table from the database by deleting
 * all temporary files created as part of this table
 *
 */
void Table::unload()
{
    logger.log("Table::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->tableName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this table
 *
 * @return Cursor
 */
Cursor Table::getCursor()
{
    logger.log("Table::getCursor");
    Cursor cursor(this->tableName, 0);
    return cursor;
}
/**
 * @brief Function that returns the index of column indicated by columnName
 *
 * @param columnName
 * @return int
 */
int Table::getColumnIndex(string columnName)
{
    logger.log("Table::getColumnIndex");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (this->columns[columnCounter] == columnName)
            return columnCounter;
    }
    return -1;
}


/**
 * @brief Check Symmetricity.
 */
void Table::isSymmetric()
{
    logger.log("Table::isSymmetric");
    bool symmetry = true;

    for (int i = 0; i < this->rowCount; i++)
    {
        for (int j = i + 1; j < this->columnCount; j++)
        {
            Cursor cursorI(this->tableName, 0);
            Cursor cursorJ(this->tableName, 0);
            vector<int> rowI{0};
            vector<int> rowJ{0};

            for (size_t iterator = 0; iterator < i + 1; iterator++)
            {
                rowI.clear();
                rowI = cursorI.getNext();
            }
            for (size_t iterator = 0; iterator < j + 1; iterator++)
            {
                rowJ.clear();
                rowJ = cursorJ.getNext();
            }


            if (rowI[j] != rowJ[i])
            {
                symmetry = false;
                break;
            }
        }

        if(!symmetry){
            break;
        }
    }
    
    if (symmetry)
    {
        cout << "TRUE : Matrix is symmetric" << endl;
    }
    else {
        cout << "FALSE : Matrix is not symmetric" << endl;
    }
    
    NO_OF_BLOCK_READ += this->rowCount / this->maxRowsPerBlock;
    NO_OF_BLOCK_READ++;
    return;
}


void Table::transposeHelper(int newValue, int rowCoordinate, int columnCoordinate){

    int pageIndex = rowCoordinate / this->maxRowsPerBlock;
    Cursor cursor(this->tableName, pageIndex);
    
    vector<int> row{0};
    vector<vector<int>> rows{row};
    rows.clear();
    
    for (size_t iterator = 0; iterator < this->maxRowsPerBlock; iterator++)
    {
        row.clear();
        row = cursor.getNext();
        rows.emplace_back(row);
    }

    int rowIndex = rowCoordinate % this->maxRowsPerBlock;
    rows[rowIndex][columnCoordinate] = newValue;
    Page p(this->tableName, pageIndex, rows, this->rowsPerBlockCount[pageIndex]);
    bufferManager.updatePage(p);
    p.writePage();
}

/**
 * @brief Tanspose the values in table.
 */
void Table::transpose()
{
    logger.log("Table::transpose");

    for (int i = 0; i < this->rowCount; i++)
    {
        for (int j = i + 1; j < this->columnCount; j++)
        {
            Cursor cursorI(this->tableName, 0);
            Cursor cursorJ(this->tableName, 0);
            vector<int> rowI{0};
            vector<int> rowJ{0};

            for (size_t iterator = 0; iterator < i + 1; iterator++)
            {
                rowI.clear();
                rowI = cursorI.getNext();
            }
            for (size_t iterator = 0; iterator < j + 1; iterator++)
            {
                rowJ.clear();
                rowJ = cursorJ.getNext();
            }
            this->transposeHelper(rowJ[i], i, j);
            this->transposeHelper(rowI[j], j, i);
        }
    }

    NO_OF_BLOCK_READ += this->rowCount / this->maxRowsPerBlock;
    NO_OF_BLOCK_READ++;
    NO_OF_BLOCK_WRITTEN += this->rowCount / this->maxRowsPerBlock;
    NO_OF_BLOCK_WRITTEN++;
    return;
}

bool compareRows(vector<int> a, vector<int> b)
{
    Table *tablePtr = tableCatalogue.getTable(srtTbName);
    for (int i = 0; i < srtAttr.size(); i++)
    {
        if (a[tablePtr->getColumnIndex(srtAttr[i])] < b[tablePtr->getColumnIndex(srtAttr[i])])
        {
            return srtStrtgy[i] == ASC;
        }
        else if (a[tablePtr->getColumnIndex(srtAttr[i])] > b[tablePtr->getColumnIndex(srtAttr[i])])
        {
            return srtStrtgy[i] == DESC;
        }
    }
    return false;
}

int getIndexOfMinRow(vector<vector<int>> rows)
{
    int index = -1;
    for (int i = 0; i < rows.size(); i++)
    {
        if (!rows[i].empty())
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        return -1;
    }
    
    for (int i = 0; i < rows.size(); i++)
    {
        if (rows[i].empty())
        {
            continue;
        }
        
        if (!compareRows(rows[index], rows[i])) 
        {
            index = i;
        }
        
    }
    
    return index;
}

void mergeHelper(vector<string> prevPassSubFileNames, vector<string> thisPassSubFileNames, int degree)
{
    bufferManager.clearPool();

    Table *tablePtr = tableCatalogue.getTable(srtTbName);
    uint totalNumOfBlocksInTable = tablePtr->blockCount;
    uint blocksPerPrevPassSubFile = ceil((double)tablePtr->blockCount / (double)prevPassSubFileNames.size());
    uint blocksLastPrevPassSubFile = totalNumOfBlocksInTable % blocksPerPrevPassSubFile;
    uint blocksPerThisPassSubFile = degree * blocksPerPrevPassSubFile;
    uint blocksLastThisPassSubFile = totalNumOfBlocksInTable % blocksPerThisPassSubFile;

    for (int thisCnt = 0; thisCnt < thisPassSubFileNames.size(); thisCnt++)
    {
        vector<Table*> prevTables;
        vector<Cursor*> prevCursors;

        for (int prevCnt = 0; prevCnt < prevPassSubFileNames.size(); prevCnt++)
        {
            if (thisCnt == prevCnt / degree)
            {                
                Table *prevTb = new Table(prevPassSubFileNames[prevCnt], tablePtr->columns, false);
                tableCatalogue.insertTable(prevTb);
                prevTb->blockCount = blocksPerPrevPassSubFile;                
                if (prevCnt == prevPassSubFileNames.size() - 1 && blocksLastPrevPassSubFile != 0)
                {
                    prevTb->blockCount = blocksLastPrevPassSubFile;
                }
                prevTb->rowCount = blocksPerPrevPassSubFile * prevTb->maxRowsPerBlock;
                if (prevCnt == prevPassSubFileNames.size() - 1 && tablePtr->rowCount % prevTb->rowCount != 0)
                {
                    prevTb->rowCount = tablePtr->rowCount % prevTb->rowCount;
                }
                prevTb->rowsPerBlockCount.clear();
                for (int x = 0; x < prevTb->blockCount; x++)
                {
                    int rowsInBlock = prevTb->maxRowsPerBlock;
                    if (x == prevTb->blockCount - 1 && prevTb->rowCount % prevTb->maxRowsPerBlock != 0)
                    {
                        rowsInBlock = prevTb->rowCount % prevTb->maxRowsPerBlock;
                    }
                    prevTb->rowsPerBlockCount.push_back(rowsInBlock);
                }
                prevTb->distinctValuesPerColumnCount = tablePtr->distinctValuesPerColumnCount;

                prevTables.push_back(prevTb);

                Cursor *prevCrsr = new Cursor(prevPassSubFileNames[prevCnt], 0);
                prevCursors.push_back(prevCrsr);

            }
        }

        vector<vector<int>> rows;
        rows.clear();
        vector<vector<int>> merged;
        merged.clear();
        int pIndex = 0;

        vector<int> row;
        for (int i = 0; i < prevTables.size(); i++)
        {
            row.clear();
            row = prevCursors[i]->getNext();
            rows.push_back(row);
        }

        int indexOfMinRow = getIndexOfMinRow(rows);
        if (indexOfMinRow != -1)
        {
            merged.push_back(rows[indexOfMinRow]);
        }
        while (true)
        {
            if (merged.size() == tablePtr->maxRowsPerBlock)
            {
                bufferManager.writePage(thisPassSubFileNames[thisCnt], pIndex, merged, merged.size());
                pIndex++;
                merged.clear();
            }

            if (indexOfMinRow == -1)
            {
                if (!merged.empty())
                {
                    bufferManager.writePage(thisPassSubFileNames[thisCnt], pIndex, merged, merged.size());
                }
                break;
            }

            row.clear();
            row = prevCursors[indexOfMinRow]->getNext();
            rows[indexOfMinRow] = row;
            indexOfMinRow = getIndexOfMinRow(rows);
            if (indexOfMinRow != -1)
            {
                merged.push_back(rows[indexOfMinRow]);
            }
        }  

        for (int prevCnt = 0; prevCnt < prevPassSubFileNames.size(); prevCnt++)
        {
            if (thisCnt == prevCnt / degree)
            {
                tableCatalogue.deleteTable(prevPassSubFileNames[prevCnt]);
            }
        }
    }
    return;
}

// Returns name of a huge sorted tempFile of size (no. of pages * pageSize) i.e. all pages are in the huge file
string Table::sortTable(vector<string> attributesToSort, vector<SortingStrategy> strategies)
{
    bufferManager.clearPool();
    const int bufferSizeInBlocks = BLOCK_COUNT;
    const int fileSizeInBlocks = this->blockCount;
    
    vector<uint> numRowsPerBlock = this->rowsPerBlockCount;
    long long int rowCount = this->rowCount;
    const int numOfInitialRuns = ceil((double)fileSizeInBlocks / (double)bufferSizeInBlocks);

    srtAttr = attributesToSort;
    srtStrtgy = strategies;
    
    vector<vector<int>> rowsToSort;
    rowsToSort.clear();

    // Sorting Phase (after this there will be ceil(fileSizeInBlocks / bufferSizeInBlocks) sorted subgroups of pages, 
    // with each subgroup having maximum bufferSizeInBlocks pages).
    // cout << "Initiating Sorting Phase ..." << endl;

    srtTbName = this->tableName;
    Cursor cursor = Cursor(this->tableName, 0);
    for (int runNm = 0; runNm < numOfInitialRuns; runNm++)
    {
        rowsToSort.clear();
        for (int pgCtr = 0; pgCtr < this->blockCount; pgCtr++)
        {
            if (runNm == pgCtr / bufferSizeInBlocks)
            {
                Cursor cursor = Cursor(this->tableName, pgCtr);
                for (int r = 0; r < this->rowsPerBlockCount[pgCtr]; r++)
                {
                    vector<int> row = cursor.getNext();
                    rowsToSort.push_back(row);
                }
            }
        }

        sort(rowsToSort.begin(), rowsToSort.end(), compareRows);

        string subFileName = this->tableName + "_tempExternalSort_run" + to_string(0) + "_subFile" + to_string(runNm);
        for (int pIndex = 0; pIndex < ceil((double)rowsToSort.size() / (double)this->maxRowsPerBlock); pIndex++)
        {
            vector<vector<int>> temp;
            for (int rCtr = 0; rCtr < rowsToSort.size(); rCtr++)
            {
                if (pIndex == rCtr / this->maxRowsPerBlock)
                {
                    temp.push_back(rowsToSort[rCtr]);
                }                
            }
            bufferManager.writePage(subFileName, pIndex, temp, temp.size());
        }
    }

    // Merging Phase
    // cout << "Initiating Merging Phase ..." << endl;

    const int degreeOfMerging = min(numOfInitialRuns, bufferSizeInBlocks - 1);
    int numOfSubfilesWrittenInPreviousPass = numOfInitialRuns;
    string returnValue = this->tableName + "_tempExternalSort_run" + to_string(0) + "_subFile" + to_string(0);

    int runCounter = 0;
    while (1)
    {
        if (numOfSubfilesWrittenInPreviousPass == 1)
        {       
            break;
        }
        int numOfSubfilesWrittenInThisPass = ceil((double)numOfSubfilesWrittenInPreviousPass / (double)(degreeOfMerging));
        
        vector<string> prevRunSubFileNames;
        vector<string> thisRunSubFileNames;
        
        for (int cnt = 0; cnt < numOfSubfilesWrittenInPreviousPass; cnt++)
        {
            prevRunSubFileNames.push_back(this->tableName + "_tempExternalSort_run" + to_string(runCounter) + "_subFile" + to_string(cnt));
        }
        runCounter++;

        for (int cnt = 0; cnt < numOfSubfilesWrittenInThisPass; cnt++)
        {
            thisRunSubFileNames.push_back(this->tableName + "_tempExternalSort_run" + to_string(runCounter) + "_subFile" + to_string(cnt));
        }
        mergeHelper(prevRunSubFileNames, thisRunSubFileNames, degreeOfMerging);

        // delete temp files from previous pass
        for (int cnt = 0; cnt < prevRunSubFileNames.size(); cnt++)
        {
            string str = prevRunSubFileNames[cnt];
            int blocksInSubFile = ceil(fileSizeInBlocks / prevRunSubFileNames.size());
            if (cnt == prevRunSubFileNames.size() - 1)
            {
                blocksInSubFile = fileSizeInBlocks % blocksInSubFile;
            }
            for (int ct = 0; ct < blocksInSubFile; ct++)
            {
                bufferManager.deleteFile(str, ct);
            }             
        }

        numOfSubfilesWrittenInPreviousPass = numOfSubfilesWrittenInThisPass;
        returnValue = thisRunSubFileNames.front();
    }
    
    // Clear global variables
    srtAttr.clear();
    srtStrtgy.clear();
    srtTbName = "";

    return returnValue;
}


