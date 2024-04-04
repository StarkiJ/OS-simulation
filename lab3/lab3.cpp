#include "headlab2.h"

using namespace std;

// 块
class Block
{
public:
    int number; // 编号
    int size;   // 大小
    int start;  // 起始地址
    int used;   // 已分配

    Block(int number, int size, int start, int used)
    {
        this->number = number;
        this->size = size;
        this->start = start;
        this->used = used;
    }
};

// 目录项(文件控制块)
class File
{
public:
    string name;    // 文件名
    int identifier; // 标识符
    int type;       // 类型
    int location;   // 位置
    int size;       // 大小
    int protection; // 保护
    int updatetime; // 更新时间

    File(string name, int identifier, int type, int protection)
    {
        this->name = name;
        this->identifier = identifier;
        this->type = type;
        this->location = location;
        this->size = size;
        this->protection = protection;
    }
};

class FileSystem
{
    vector<int> space;           // 外存空间
    vector<int> memory;          // 内存
    vector<File> directory;      // 目录
    vector<Block> spaceBlocks;   // 外存空间块列表
    vector<Block> memoryBlocks;  // 内存块列表
    vector<int> freeSpaceBlocks; // 空闲外存空间块列表
    vector<int> freeMemBlocks;   // 空闲内存块列表

    int blockSize;
    int numOfSpaceBlocks; // 外存空间块数量
    int numOfMemBlocks;   // 内存块数量
    int numOfFiles;       // 文件数量

public:
    FileSystem(int sizeOfSpace, int sizeOfMemory, int blockSize)
    {
        space = vector<int>(sizeOfSpace, -1);
        memory = vector<int>(sizeOfMemory, -1);
        this->blockSize = blockSize;
        numOfSpaceBlocks = sizeOfSpace / blockSize;
        numOfMemBlocks = sizeOfMemory / blockSize;
        numOfFiles = 0;

        for (int i = 0; i < numOfSpaceBlocks; i++)
        {
            spaceBlocks.push_back(Block(i, blockSize, i * blockSize, 0));
        }
        for (int i = 0; i < numOfMemBlocks; i++)
        {
            memoryBlocks.push_back(Block(i, blockSize, i * blockSize, 0));
        }
    }

    // 寻找空闲外存空间块
    int findFreeBlock()
    {
        if (freeSpaceBlocks.size() == 0)
        {
            cout << "空间不足，寻找空闲块失败" << endl;
            return;
        }
    }

    // 创建文件，指明文件名、标识符、类型、位置、大小、保护属性：1.分配空间，2.建立目录项
    void createFile(string name, int type, int protection)
    {
        File newFile(name, numOfFiles, type, protection);
        numOfFiles++;
    }

    // 写文件，指明文件名和要写入文件的内容：1.在目录中搜索，2.利用目录的文件指针写文件
    void writeFile(string name, int data)
    {
    }

    // 读文件，指明文件名和要读入文件块的内存位置：1.在目录中搜索，2.将文件内容读入内存
    void readFile(string name, int start)
    {
    }

    // 删除文件，指明文件名:1.在目录中搜索，2.释放空间，3.删除目录项
    void deleteFile(string name)
    {
    }

    // 打开文件，指明文件名：1.将目录信息读入内存打开文件表中，建立起用户进程和文件之间的联系。
    void openFile(string name)
    {
    }

    // 关闭文件，指明文件名：1.撤销主存中的目录信息，并释放用户进程和文件之间的联系，2.考虑是否写回文件
    void closeFile(string name)
    {
    }
};

int main()
{

    return 0;
}
