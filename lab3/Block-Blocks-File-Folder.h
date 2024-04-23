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
        if (index < 0 || index >= num)
        {
            cout << "错误：清除目标为非法块序号！！！" << endl;
            return false;
        }
        for (int i = 0; i < free.size(); i++)
        {
            if (free[i] == index)
            {
                cout << "错误：重复清除空闲块！！！" << endl;
                return false;
            }
        }

        free.push_back(index);
        if (blocks[index].used == size) // 如果不是这个文件的最后一个块
        {
            blocks[index].used = 0;
            return false;
        }

        blocks[index].used = 0;
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
    int location;      // 位置(块号)
    int size;          // 大小
    int protection;    // 保护
    time_t updatetime; // 更新时间

    File(string name, int id, int type, int location, int protection)
    {
        this->name = name;
        this->id = id;
        this->type = type;
        this->location = location;
        this->size = 1;
        this->protection = protection;
        this->updatetime = time(NULL);
    }
};

class MemFile
{
public:
    File *file; // 外存文件指针
    int memLoc; // 内存位置(块号)
    int size;   // 文件大小(块数)
    bool dirty; // 脏位

    MemFile() {}
    MemFile(File *file, int memLoc)
    {
        this->file = file;
        this->memLoc = memLoc;
        this->size = file->size;
        this->dirty = false;
    }
};

// 树状目录
class Folder
{
public:
    string path;
    string name;
    int protection; // 保护

    vector<File> files;
    vector<Folder> folders;

    Folder() {}
    Folder(string path, string name, int protection)
    {
        this->path = path;
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

    void showFolder()
    {
        int folderNum = folders.size();
        int fileNum = files.size();

        if (folderNum == 0 && fileNum == 0)
        {
            return;
        }

        cout << path << "/" << name << endl;

        cout << "文件夹(" << folderNum << "): ";
        for (int i = 0; i < folderNum; i++)
        {
            cout << folders[i].name << " ";
        }
        cout << endl;

        cout << "文件(" << fileNum << "): ";
        for (int i = 0; i < fileNum; i++)
        {
            cout << files[i].name
                 << "(" << files[i].id << ")"
                 << " ";
        }
        cout << endl;

        cout << endl;
    }

    // 显示树状目录
    void showAll()
    {
        int folderNum = folders.size();

        showFolder();

        for (int i = 0; i < folderNum; i++)
        {
            folders[i].showAll();
        }
    }

    // 写入文件
    void writeBack(ofstream &ofs)
    {
        int folderNum = folders.size();
        int fileNum = files.size();

        ofs << path << endl;
        ofs << name << endl;
        ofs << protection << " " << folderNum << " " << fileNum << endl;

        for (int i = 0; i < fileNum; i++)
        {
            ofs << files[i].name << endl;
            ofs << files[i].id << " "
                << files[i].type << " "
                << files[i].location << " "
                << files[i].size << " "
                << files[i].protection << " "
                << files[i].updatetime << endl;
        }

        for (int i = 0; i < folderNum; i++) // 递归写入文件夹
        {
            folders[i].writeBack(ofs);
        }
    }

    // 读取文件
    void readBack(ifstream &ifs)
    {
        int folderNum;
        int fileNum;
        getline(ifs, path);
        getline(ifs, name);
        ifs >> protection >> folderNum >> fileNum;
        ifs.ignore();
        for (int i = 0; i < fileNum; i++)
        {
            string name;
            int id;
            int type;
            int location;
            int size;
            int protection;
            time_t updatetime;
            getline(ifs, name);
            ifs >> id >> type >> location >> size >> protection >> updatetime;
            ifs.ignore();
            File ftmp(name, id, type, location, protection);
            ftmp.size = size;
            ftmp.updatetime = updatetime;
            files.push_back(ftmp);
        }

        for (int i = 0; i < folderNum; i++) // 递归读取文件夹
        {
            Folder folder;
            folder.readBack(ifs);
            folders.push_back(folder);
        }
    }
};