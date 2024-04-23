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
        if (index < 0 || index >= num)
        {
            cout << "�������Ŀ��Ϊ�Ƿ�����ţ�����" << endl;
            return false;
        }
        for (int i = 0; i < free.size(); i++)
        {
            if (free[i] == index)
            {
                cout << "�����ظ�������п飡����" << endl;
                return false;
            }
        }

        free.push_back(index);
        if (blocks[index].used == size) // �����������ļ������һ����
        {
            blocks[index].used = 0;
            return false;
        }

        blocks[index].used = 0;
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
        this->updatetime = time(NULL);
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
    string path;
    string name;
    int protection; // ����

    vector<File> files;
    vector<Folder> folders;

    Folder() {}
    Folder(string path, string name, int protection)
    {
        this->path = path;
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

    void showFolder()
    {
        int folderNum = folders.size();
        int fileNum = files.size();

        if (folderNum == 0 && fileNum == 0)
        {
            return;
        }

        cout << path << "/" << name << endl;

        cout << "�ļ���(" << folderNum << "): ";
        for (int i = 0; i < folderNum; i++)
        {
            cout << folders[i].name << " ";
        }
        cout << endl;

        cout << "�ļ�(" << fileNum << "): ";
        for (int i = 0; i < fileNum; i++)
        {
            cout << files[i].name
                 << "(" << files[i].id << ")"
                 << " ";
        }
        cout << endl;

        cout << endl;
    }

    // ��ʾ��״Ŀ¼
    void showAll()
    {
        int folderNum = folders.size();

        showFolder();

        for (int i = 0; i < folderNum; i++)
        {
            folders[i].showAll();
        }
    }

    // д���ļ�
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

        for (int i = 0; i < folderNum; i++) // �ݹ�д���ļ���
        {
            folders[i].writeBack(ofs);
        }
    }

    // ��ȡ�ļ�
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

        for (int i = 0; i < folderNum; i++) // �ݹ��ȡ�ļ���
        {
            Folder folder;
            folder.readBack(ifs);
            folders.push_back(folder);
        }
    }
};