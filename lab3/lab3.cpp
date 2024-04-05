#include "headlab2.h"

using namespace std;

// 块
class Block
{
public:
    int num;   // 编号
    int size;  // 大小
    int start; // 起始地址
    int used;  // 已分配

    Block(int num, int size, int start, int used)
    {
        this->num = num;
        this->size = size;
        this->start = start;
        this->used = used;
    }
};

class Blocks
{
public:
    vector<Block> blocks; // 块列表
    vector<int> free;     // 空闲块列表
    int num;              // 块数量

    Blocks() {}
    Blocks(int space, int size)
    {
        num = space / size;
        for (int i = 0; i < num; i++)
        {
            blocks.push_back(Block(i, size, i * size, 0));
            free.push_back(i);
        }
    }

    // 寻找空闲外存空间块
    int findFreeBlock()
    {
        if (free.size() == 0)
        {
            return -1;
        }

        int num = free.front();
        free.erase(free.begin());
        return num;
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

    File(string name, int identifier, int type, int location, int protection)
    {
        this->name = name;
        this->identifier = identifier;
        this->type = type;
        this->location = location;
        this->size = 0;
        this->protection = protection;
    }
};

// 树状目录
class Folder
{
public:
    string name;

    vector<File> files;
    vector<Folder> folders;

    Folder() {}
    Folder(string name)
    {
        this->name = name;
    }

    bool getPath(string path) // 获取路径
    {
    }

    int findFolder(string name) // 获取文件
    {
        int size = folders.size();
        for (int j = 0; j < size; j++)
        {
            if (folders[j].name == name)
            {
                return j;
            }
            return -1;
        }
    }
};

class FileSystem
{
    vector<int> space;  // 外存空间
    vector<int> memory; // 内存

    Blocks spaceBlocks; // 外存空间块
    Blocks memBlocks;   // 内存块

    int blockSize;  // 块大小
    int numOfFiles; // 文件数量

    Folder root;       // 根目录
    Folder *curFolder; // 当前目录
    string curPath;    // 当前路径

public:
    FileSystem(int sizeOfSpace, int sizeOfMemory, int blockSize)
    {
        space = vector<int>(sizeOfSpace, -1);
        memory = vector<int>(sizeOfMemory, -1);

        this->blockSize = blockSize;
        numOfFiles = 0;

        spaceBlocks = Blocks(sizeOfSpace, blockSize);
        memBlocks = Blocks(sizeOfMemory, blockSize);

        root = Folder("root");
        curFolder = &root;
        curPath = "";
    }

    bool openFolder(string path)
    {
        string name = "";           // 文件夹的名字
        string tmpPath = curPath;   // 临时存放当前路径
        Folder *tmpPtr = curFolder; // 临时指针存放当前目录
        int len = path.length();    // 路径长度

        // 判断是绝对路径还是相对路径
        if (path[0] == '/') // 绝对路径
        {
            curPath = path.substr(0, len - 1);
            curFolder = &root;
        }
        else if (path[1] == '.') // 相对路径：上一级目录
        {
            if (curPath.length() == 0)
            {
                cout << "打开文件夹失败，当前文件夹没有父文件夹" << endl;
                return false;
            }

            int curlen = curPath.length(); // 当前路径长度
            for (int i = curlen; i > 0; i--)
            {
                if (curPath[i] == '/')
                {
                    if (!openFolder(curPath.substr(0, i)))
                    {
                        cout << "打开文件夹失败，当前文件夹的父文件夹路径错误" << endl;
                        return false;
                    }
                    curPath = curPath.substr(0, i) + path.substr(2, len - 1);
                    break;
                }
            }
        }
        else if (path[0] == '.') // 相对路径：当前目录
        {
            curPath += path.substr(1, len - 1);
        }

        for (int i = 0; i < len; i++)
        {
            if (path[i] == '.') // 跳过点
            {
                continue;
            }
            if (path[i] != '/') // 是名字
            {
                name += path[i];
                if (i != len - 1) // 最后一个没有'/'结尾
                {
                    continue;
                }
            }

            if (name != "")
            {
                int num = curFolder->findFolder(name);
                if (num == -1)
                {
                    cout << "打开文件夹失败，没有找到文件夹: " << path.substr(0, i) << endl;
                    curPath = tmpPath;  // 回到当前路径
                    curFolder = tmpPtr; // 回到当前目录
                    return false;
                }
                curFolder = &curFolder->folders[num];
                name = "";
            }
        }

        return true;
    }

    // 创建文件，指明文件名、标识符、类型、位置、大小、保护属性：1.分配空间，2.建立目录项
    void createFile(string name, int type, int protection)
    {
        int location = spaceBlocks.findFreeBlock();
        if (location == -1)
        {
            cout << "空间不足，创建文件失败" << endl;
            return;
        }
        File newFile(name, numOfFiles, type, location, protection);
        curFolder->files.push_back(newFile);
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
