/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file datablocks.h
 */

#include <vector>
#include <set>
#include <stdio.h>
#include <string>

/**
 * Class representing Data pakets colected during multicast and operations with them
 */
class DataBlocks{
private:
    struct DataBlock{
        int packetNumber;
        std::string data;
        bool operator<(const DataBlock &dataBlock) const { return packetNumber < dataBlock.packetNumber; }
    };

    std::set<DataBlock> allDataBlocks;
    unsigned long fileSize = 0;
    unsigned long dataBlocksSize = 0;

public:
    DataBlocks(unsigned long fileSize);
    void AddDataBlock(int packetNumber, char* data);
    void fprintfDataBlocks(FILE *f);
    void Sort();
    int GetFirstMissing();
    bool isDownloaded();
    unsigned long getDataBlockSize();

};