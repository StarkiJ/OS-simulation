#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

// 块
class Block
{
public:
    int start; // 起始地址
    int used;  // 已分配

    Block(int start)
    {
        this->start = start;
        this->used = 0;
    }
};

class Blocks
{
public:
    vector<Block> blocks; // 块列表
    vector<int> free;     // 空闲块列表

    int size; // 块大小
    int num;  // 块数量

    Blocks() {}
    Blocks(int space, int size)
    {
        this->size = size;
        this->num = space / size;

        for (int i = 0; i < num; i++)
        {
            blocks.push_back(Block(i * size));
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

        int index = free.front(); // 取出第一个空闲块序号
        free.erase(free.begin());

        blocks[index].used = 0; // 已分配空间置0

        return index; // 这里记得可能可以改动
    }

    // 释放块
    bool freeBlock(int index)
    {
        free.push_back(index);
        if (blocks[index].used == size) // 如果不是这个文件的最后一个块
        {
            return false;
        }

        return true;
    }
};

// 目录项(文件控制块)
class File
{
public:
    string name;       // 文件名
    int id;            // 标识符
    int type;          // 类型
    int location;      // 位置
    int size;          // 大小
    int protection;    // 保护
    time_t updatetime; // 更新时间

    File(string name, int id, int type, int location, int protection)
    {
        this->name = name;
        this->id = id;
        this->type = type;
        this->location = location;
        this->size = 0;
        this->protection = protection;
        time(&updatetime);
    }
};

// 树状目录
class Folder
{
public:
    string name;
    int protection; // 保护

    vector<File> files;
    vector<Folder> folders;

    Folder() {}
    Folder(string name, int protection)
    {
        this->name = name;
        this->protection = protection;
    }

    int findFolder(string name) // 获取文件夹编号
    {
        int size = folders.size();
        for (int j = 0; j < size; j++)
        {
            if (folders[j].name == name)
            {
                return j;
            }
        }
        return -1;
    }

    int findFile(string name) // 获取文件编号
    {
        int size = files.size();
        for (int j = 0; j < size; j++)
        {
            if (files[j].name == name)
            {
                return j;
            }
        }
        return -1;
    }
};