#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

// ��
class Block
{
public:
    int start; // ��ʼ��ַ
    int used;  // �ѷ���

    Block(int start)
    {
        this->start = start;
        this->used = 0;
    }
};

class Blocks
{
public:
    vector<Block> blocks; // ���б�
    vector<int> free;     // ���п��б�

    int size; // ���С
    int num;  // ������

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

    // Ѱ�ҿ������ռ��
    int findFreeBlock()
    {
        if (free.size() == 0)
        {
            return -1;
        }

        int index = free.front(); // ȡ����һ�����п����
        free.erase(free.begin());

        blocks[index].used = 0; // �ѷ���ռ���0

        return index; // ����ǵÿ��ܿ��ԸĶ�
    }

    // �ͷſ�
    bool freeBlock(int index)
    {
        for (int i = 0; i < free.size(); i++)
        {
            if (free[i] == index)
            {
                cout << "�����ظ�������п飡����" << endl;
                return false;
            }
        }

        blocks[index].used = 0;
        free.push_back(index);
        if (blocks[index].used == size) // �����������ļ������һ����
        {
            return false;
        }

        return true;
    }
};

// Ŀ¼��(�ļ����ƿ�)
class File
{
public:
    string name;       // �ļ���
    int id;            // ��ʶ��
    int type;          // ����
    int location;      // λ��(���)
    int size;          // ��С
    int protection;    // ����
    time_t updatetime; // ����ʱ��

    File(string name, int id, int type, int location, int protection)
    {
        this->name = name;
        this->id = id;
        this->type = type;
        this->location = location;
        this->size = 1;
        this->protection = protection;
        time(&updatetime);
    }
};

class MemFile
{
public:
    File *file; // ����ļ�ָ��
    int memLoc; // �ڴ�λ��(���)
    int size;   // �ļ���С(����)
    bool dirty; // ��λ

    MemFile() {}
    MemFile(File *file, int memLoc)
    {
        this->file = file;
        this->memLoc = memLoc;
        this->size = file->size;
        this->dirty = false;
    }
};

// ��״Ŀ¼
class Folder
{
public:
    string name;
    int protection; // ����

    vector<File> files;
    vector<Folder> folders;

    Folder() {}
    Folder(string name, int protection)
    {
        this->name = name;
        this->protection = protection;
    }

    int findFolder(string name) // ��ȡ�ļ��б��
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

    int findFile(string name) // ��ȡ�ļ����
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