//
//  SimpleGoogFileSystem.cpp
//  AlgoTest
//
//  Created by Tony Chen on 12/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>

using namespace std;

// 本问题就是举例说明一下GFS中文件如何分成各个Chunk存在具体的机器／硬盘中。当一个文件要写入GFS时，首先查询该文件对应的chunk是否存在。本算法里，这一步通过
// 查询一个filename到chunkNum的map实现。chunkNum指的是该文件对应的chunk的数量。如果文件存在0，1，2chunks中，则chunkNum为3。然后，文件字符串就被根据预设的
// chunksize分割，依次写入一个一个chunk。chunkSize在实际系统中为64M。读取的过程也类似，先从map中获得filename的chunkNum，然后从0到chunkNum－1的Chunk 中依次
// 读。
//

class BaseGFSClient {
public:
    string readChunk(string const & filename, int chunkIndex) {
        // Read a chunk from GFS(actually the map here)
        string output;
        auto itContent = mapFileChunkContent.find(filename);
        if (itContent != mapFileChunkContent.end()) {
            auto itChunkContent = itContent->second.find(chunkIndex);
            if (itChunkContent != itContent->second.end()) {
                output = itChunkContent->second;
            }
        }
        return output;
    }
    
    void writeChunk(string const & filename, int chunkIndex, string content) {
        // Write a chunk to GFS(actually the map here)
        mapFileChunkContent[filename][chunkIndex] = std::move(content); // move in the content straightaway here
    }
    
    
private:
    unordered_map<string, unordered_map<int, string>> mapFileChunkContent;
};

class GFSClient: public BaseGFSClient {
public:
    GFSClient(int chunkSize) : chunkSize(chunkSize) {}
    
    string read (string const & filename) {
        string output;
        auto itChunkNum = mapFileNameToChunkNum.find(filename);
        if (itChunkNum != mapFileNameToChunkNum.end()) {
            for (int i = 0; i < itChunkNum->second; i++) {
                output += readChunk(filename, i);
            }
        }
        return output;
    }
    
    void write(string const & filename, string const& content) {
        int nContentPos = 0;
        int chunkIdx = -1;
        
        auto itChunkNum = mapFileNameToChunkNum.find(filename);
        if (itChunkNum == mapFileNameToChunkNum.end())
            itChunkNum = mapFileNameToChunkNum.emplace(filename, 0).first;
        
        
        while (nContentPos < content.size()) {
            chunkIdx++;
            writeChunk(filename, chunkIdx, content.substr(nContentPos, chunkSize));
            nContentPos += chunkSize;
        }
        
        itChunkNum->second = chunkIdx+1;
    }
private:
    int chunkSize;
    unordered_map<string, int> mapFileNameToChunkNum;
    
};

TEST(SimpleGFS, NoData) {
    GFSClient gfs(5);
    string filename = "test.txt";
    gfs.write(filename, "aaaaabbbbbccc");
    EXPECT_EQ(gfs.read(filename), "aaaaabbbbbccc");
    
    
}
