/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file datablocks.cpp
 */

#include "datablocks.h"
#include "string.h"

/**
 * @brief Constructor
 * @param fileSize size of file
 */
DataBlocks::DataBlocks(unsigned long fileSize){
    this->fileSize = fileSize;
}

/**
 * @brief Inserts new datablock to ordered set with all data blocks
 * @param packetNumber number of data packet
 * @param data buffer with data  
 */
void DataBlocks::AddDataBlock(int packetNumber, char* data){
    dataBlocksSize += strlen(data);
    allDataBlocks.insert({packetNumber, data});
}

/**
 * Writes data from all data blocks to file
 * @param f file where to put data
 */
void DataBlocks::fprintfDataBlocks(FILE *f){
    for (DataBlock dataBlock: allDataBlocks)
    {
        fwrite(dataBlock.data.c_str(), dataBlock.data.size(), 1, f);
    }
}

void DataBlocks::Sort(){
    return;
}

/**
 * Gets number of first missing data packet
 * @return number of first missing data packet
 */
int DataBlocks::GetFirstMissing(){
    
    int next = 1;

    for (DataBlock dataBlock: allDataBlocks)
    {
        if(dataBlock.packetNumber == next)
            next++;
        else
            return next;
    }
    // unreachable because of usage
    return -1;
}

/**
 * @return true if file is downloaded i.e size of all data blocks is equal
 * to file size, false otherwise
 */
bool DataBlocks::isDownloaded(){
    return dataBlocksSize == fileSize;
}

/**
 * @return size of all data blocks present in structure
 */
unsigned long DataBlocks::getDataBlockSize(){
    return dataBlocksSize;
}