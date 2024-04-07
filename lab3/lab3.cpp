#include "headlab2.h"

using namespace std;

// ��
class Block
{
public:
    int num;   // ���
    int start; // ��ʼ��ַ
    int used;  // �ѷ���

    Block(int num, int start)
    {
        this->num = num;
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
            blocks.push_back(Block(i, i * size));
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

        return index;
    }

    // �ͷſ�
    int freeBlock(int index)
    {
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
    string name;    // �ļ���
    int identifier; // ��ʶ��
    int type;       // ����
    int location;   // λ��
    int size;       // ��С
    int protection; // ����
    int updatetime; // ����ʱ��

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

// ��״Ŀ¼
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

class FileSystem
{
    string password; // ����
    int authority;   // Ȩ�޼���

    vector<int> space;  // ���ռ�
    vector<int> memory; // �ڴ�

    vector<File> openFiles; // �ڴ���ļ���

    Blocks spaceBlocks; // ���ռ��
    Blocks memBlocks;   // �ڴ��

    int blockSize;  // ���С
    int numOfFiles; // ��ʷ�ļ�����(����ɾ���ļ�)

    Folder root;       // ��Ŀ¼
    Folder *curFolder; // ��ǰ�ļ���ָ��
    string curPath;    // ��ǰ·��

public:
    FileSystem(int sizeOfSpace, int sizeOfMemory, int blockSize)
    {
        password = "admin";
        authority = 2;

        space = vector<int>(sizeOfSpace, -1);
        memory = vector<int>(sizeOfMemory, -1);

        this->blockSize = blockSize;
        numOfFiles = 0;

        spaceBlocks = Blocks(sizeOfSpace, blockSize);
        memBlocks = Blocks(sizeOfMemory, blockSize);

        root = Folder("root");
        curFolder = &root;
        curPath = "root:";
    }

    // ��������
    void setPassword()
    {
        getline(cin, password);
    }

    // ��������Ȩ��
    void adjustAuthority(string word)
    {
        if (word != password)
        {
            cout << "�������" << endl;
            return;
        }

        cout << "��ѡ��Ȩ�޼���" << endl;
        cout << "0.��ֹ����" << endl;
        cout << "1.ֻ��" << endl;
        cout << "2.��д" << endl;
        cin >> authority;
    }

    // �ж��Ƿ�Υ��
    bool ifIllegal(int limit)
    {
        if (authority < limit)
        {
            cout << "Ȩ�޲���" << endl;
            return true;
        }
        return false;
    }

    // ��ʾ��ǰ·��
    void showPath()
    {
        cout << "��ǰ·����" << curPath << endl;
    }

    // ��ʾ��ǰ�ļ�������
    void showThisFolder()
    {
        showPath();

        cout << "�ļ��У�";
        int size = curFolder->folders.size();
        for (int i = 0; i < size; i++)
        {
            cout << curFolder->folders[i].name << " ";
        }
        cout << endl;

        cout << "�ļ���";
        size = curFolder->files.size();
        for (int i = 0; i < size; i++)
        {
            cout << curFolder->files[i].name << " ";
        }
        cout << endl;
    }

    // ���ļ���
    bool openFolder(string path)
    {
        string name = "";              // �ļ��е�����
        string tmpPath = curPath;      // ��ʱ·��
        Folder *tmpFolder = curFolder; // ��ʱ�ļ���ָ��
        int len = path.length();       // ·������

        // ��ֹ����·���д���"root:"
        if (path.substr(0, 5) == "root:") // ��ֹ����·���д���"root:"
        {
            if (len == 5)
            {
                path += "/";
            }
            path = path.substr(5, len);
            len = path.length();
        }

        // �ж�·����ʽ
        if (path[0] == '/') // ����·��
        {
            if (path == "/") // �����������ֹ"/"����
            {
                tmpPath = "root:";
            }
            else
            {
                tmpPath = "root:" + path;
            }
            curFolder = &root;
        }
        else if (path.substr(0, 2) == "./") // ���·������ǰĿ¼
        {
            if (path != "./")
            {
                tmpPath += path.substr(1, len);
            }
            else
            {
                cout << "??? ԭ��TP ???" << endl;
                return false;
            }
        }
        else if (path.substr(0, 3) == "../") // ���·������һ��Ŀ¼
        {
            if (curPath == "root:")
            {
                cout << "���ļ���ʧ�ܣ���Ŀ¼û�и��ļ���" << endl;
                return false;
            }

            int curlen = curPath.length(); // ��ǰ·������
            for (int i = curlen; i > 0; i--)
            {
                if (curPath[i] == '/')
                {
                    if (!openFolder(curPath.substr(0, i)))
                    {
                        cout << "���ļ���ʧ�ܣ����ļ���·������" << endl;
                        return false;
                    }

                    if (path == "../") // �������
                    {
                        tmpPath = curPath.substr(0, i);
                    }
                    else
                    {
                        tmpPath = curPath.substr(0, i) + path.substr(2, len);
                    }
                    break;
                }
            }
        }
        else
        {
            cout << "���ļ���ʧ�ܣ�·���������" << endl;
            return false;
        }

        // ����·����Ѱ��Ŀ���ļ���
        for (int i = 0; i < len; i++)
        {
            if (path[i] == '.') // ������
            {
                continue;
            }
            if (path[i] != '/') // ������
            {
                name += path[i];
                if (i != len - 1) // ���һ��û��'/'��β
                {
                    continue;
                }
                i++; // ��ֹ���һλ�޷����
            }

            if (name != "")
            {
                int index = curFolder->findFolder(name);
                if (index == -1)
                {
                    cout << "���ļ���ʧ�ܣ�û���ҵ��ļ���: " << path.substr(0, i) << endl;
                    curFolder = tmpFolder; // �����ļ���ָ��
                    return false;
                }
                curFolder = &curFolder->folders[index];
                name = "";
            }
        }

        curPath = tmpPath; // ����·��

        return true;
    }

    // �����ļ���
    bool createFolder(string name)
    {
        // �ж��ļ��������Ƿ��ظ�
        if (curFolder->findFolder(name) != -1)
        {
            cout << "�����ļ���ʧ�ܣ��ļ����Ѵ���" << endl;
            return false;
        }
        Folder newFolder(name);
        curFolder->folders.push_back(newFolder);

        cout << "�����ļ��гɹ���" << curPath << "/" << name << endl;
        return true;
    }

    // �����ļ���ָ���ļ�������ʶ�������͡�λ�á���С���������ԣ�1.����ռ䣬2.����Ŀ¼��
    bool createFile(string name, int type, int protection)
    {
        // �ж��ļ����Ƿ��ظ�
        if (curFolder->findFile(name) != -1)
        {
            cout << "�����ļ�ʧ�ܣ��ļ��Ѵ���" << endl;
            return false;
        }

        // �жϿռ��Ƿ��㹻
        int location = spaceBlocks.findFreeBlock();
        if (location == -1)
        {
            cout << "�����ļ�ʧ�ܣ��ռ䲻��" << endl;
            return false;
        }
        File newFile(name, numOfFiles, type, location, protection);
        curFolder->files.push_back(newFile);
        numOfFiles++;

        cout << "�����ļ��ɹ���" << curPath << "/" << name << endl;
        return true;
    }

    // д�ļ���ָ���ļ�����Ҫд���ļ������ݣ�1.��Ŀ¼��������2.����Ŀ¼���ļ�ָ��д�ļ�
    void writeFile(string name, int data)
    {
    }

    // ���ļ���ָ���ļ�����Ҫ�����ļ�����ڴ�λ�ã�1.��Ŀ¼��������2.���ļ����ݶ����ڴ�
    void readFile(string name, int start)
    {
    }

    // ɾ���ļ���ָ���ļ���:1.��Ŀ¼��������2.�ͷſռ䣬3.ɾ��Ŀ¼��
    bool deleteFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "ɾ���ļ�ʧ�ܣ�û���ҵ��ļ�" << endl;
            return false;
        }

        // �ͷſռ�
        int location = curFolder->files[index].location;
        while (!spaceBlocks.freeBlock(location))
        {
            location = space[location * blockSize + blockSize - 1];
        }

        // ɾ��Ŀ¼��
        curFolder->files.erase(curFolder->files.begin() + index);
    }

    // ���ļ���ָ���ļ�����1.��Ŀ¼��Ϣ�����ڴ���ļ����У��������û����̺��ļ�֮�����ϵ��
    bool openFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "���ļ�ʧ�ܣ�û���ҵ��ļ�" << endl;
            return false;
        }
    }

    // �ر��ļ���ָ���ļ�����1.���������е�Ŀ¼��Ϣ�����ͷ��û����̺��ļ�֮�����ϵ��2.�����Ƿ�д���ļ�
    void closeFile(string name)
    {
    }
};

// ��ʾ�����˵�
void showMenu()
{
    cout << "1.���ļ���" << endl;
    cout << "2.�����ļ���" << endl;
    cout << "3.�����ļ�" << endl;
    cout << "4.ɾ���ļ�" << endl;
    cout << "5.���ļ�" << endl;
    cout << "6.�ر��ļ�" << endl;
    cout << "7.��������Ȩ��" << endl;
    cout << "8.��ʾ�����˵�" << endl;
    cout << "9.�˳�" << endl;
}

int main()
{
    FileSystem myFileSys(1024, 64, 8);

    cout << "�ļ�ϵͳ�ѿ�ʼ����" << endl;
    cout << "���������룺";
    myFileSys.setPassword();

    showMenu();

    while (1)
    {
        string stmp;
        char ctmp;
        int itmp;

        cout << "������ָ�";
        cin >> ctmp;

        cin.ignore();

        switch (ctmp)
        {
        case '1':
            cout << "( /x Ϊ����·��, ./xΪ ��ǰĿ¼, ../x Ϊ��һ��Ŀ¼ )" << endl;
            cout << "�������ļ���·����";
            getline(cin, stmp);
            myFileSys.openFolder(stmp);
            myFileSys.showThisFolder(); // ��ʾ��ǰ�ļ�������
            break;
        case '2':
            cout << "�����봴�����ļ������ƣ�";
            getline(cin, stmp);
            myFileSys.createFolder(stmp);
            break;
        case '3':
            cout << "�����봴�����ļ����ƣ�";
            getline(cin, stmp);
            cout << "�����뱣���ȼ���";
            cin >> itmp;
            myFileSys.createFile(stmp, 0, itmp);
            break;
        case '4':
            cout << "������ɾ�����ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.deleteFile(stmp);
            break;
        case '5':
            cout << "������򿪵��ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.openFile(stmp);
            break;
        case '6':
            cout << "������رյ��ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.closeFile(stmp);
            break;
        case '7':
            cout << "���������룺";
            getline(cin, stmp);
            myFileSys.adjustAuthority(stmp);
            break;
        case '8':
            showMenu();
            break;
        case '9':
            system("pause");
            return 0;
        default:
            cout << "�����������������" << endl;
            break;
        }
    }

    cout << "�����쳣�˳�!!!" << endl;
    system("pause");
    return 0;
}
