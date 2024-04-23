#include "Block-Blocks-File-Folder.h"

class FileSystem
{
    string password; // ����
    int authority;   // Ȩ�޼���

    int blockSize;  // ���С
    int numOfFiles; // ��ʷ�ļ�����(����ɾ���ļ�)

    vector<int> space;  // ���ռ�
    vector<int> memory; // �ڴ�

    vector<MemFile> memFiles; // �ڴ��ļ�����

    Blocks spaceBlocks; // ���ռ��
    Blocks memBlocks;   // �ڴ��

    Folder root;       // ��Ŀ¼
    Folder *curFolder; // ��ǰ�ļ���ָ��
    string curPath;    // ��ǰ·��

    File *curFile;    // ��ǰ�ļ�ָ��
    MemFile *tmpFile; // ��ǰ�ڴ��ļ�ָ��

public:
    FileSystem(int sizeOfSpace, int sizeOfMemory, int blockSize)
    {
        password = "admin";
        authority = 2;

        this->blockSize = blockSize;
        numOfFiles = 0;

        space = vector<int>(sizeOfSpace, -1);
        memory = vector<int>(sizeOfMemory, -1);

        spaceBlocks = Blocks(sizeOfSpace, blockSize);
        memBlocks = Blocks(sizeOfMemory, blockSize);

        root = Folder(".", "root", 0);
        curFolder = &root;
        curPath = "root:";
    }

    // ��������
    void setPassword()
    {
        cout << "���������룺";
        getline(cin, password);
    }

    // ����Ա����
    void adjustAuthority(string word)
    {
        if (word != password)
        {
            cout << "�������" << endl;
            return;
        }

        cout << "0.�ο�Ȩ��" << endl;
        cout << "1.�û�Ȩ��" << endl;
        cout << "2.����ԱȨ��" << endl;
        cout << "3.�޸�����" << endl;
        cout << "��ѡ��Ȩ�޼���";
        int itmp;
        while (1)
        {
            cin >> itmp;
            cin.ignore();
            if (itmp < 0 || itmp > 3)
            {
                cout << "����������������룡" << endl;
            }
            if (itmp == 3)
            {
                setPassword();
                break;
            }
            else
            {
                authority = itmp;
                break;
            }
        }
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
        curFolder->showFolder();
    }

    // ��ʾ��״Ŀ¼
    void showTree()
    {
        root.showAll();
    }

    // ��ʼ��ȡ����
    void start()
    {
        int size = 0;
        int itmp;
        string stmp;
        ifstream ifs;

        ifs.open("info.txt", ios::in);
        if (ifs.is_open())
        {
            // �ռ��С�����������ļ���Ч
            if ((!(ifs >> size) || size != space.size()) ||
                (!(ifs >> size) || size != memory.size()) ||
                (!(ifs >> size) || size != blockSize))
            {
                cout << "�����ļ���Ч" << endl;
                return;
            }

            cout << "�����ļ���Ч���Ƿ��ȡ(Y/N)��";
            cin >> stmp;
            if (stmp == "N" || stmp == "n")
            {
                ifs.close();
                cout << "������ȡ�����ļ����½��ļ�ϵͳ" << endl;
                return;
            }

            ifs.ignore();
            getline(ifs, password);
            ifs >> numOfFiles;
        }
        else
        {
            cout << "δ�ҵ�info�ļ�" << endl;
            return;
        }
        ifs.close();

        ifs.open("space.txt", ios::in);
        if (!ifs.is_open())
        {
            cout << "��space�ļ�ʧ��" << endl;
            return;
        }
        size = space.size(); // ��ȡ���ռ�
        for (int i = 0; i < size; i++)
        {
            ifs >> itmp;
            space[i] = itmp;
        }
        ifs.close();

        ifs.open("spaceBlocks.txt", ios::in);
        if (!ifs.is_open())
        {
            cout << "��spaceBlocks�ļ�ʧ��" << endl;
            return;
        }

        spaceBlocks.free.clear();
        ifs >> size;
        for (int i = 0; i < size; i++)
        {
            ifs >> itmp;
            spaceBlocks.free.push_back(itmp);
        }

        size = spaceBlocks.blocks.size();
        for (int i = 0; i < size; i++)
        {
            ifs >> spaceBlocks.blocks[i].start >> spaceBlocks.blocks[i].used;
        }
        ifs.close();

        ifs.open("tree.txt", ios::in);
        if (!ifs.is_open())
        {
            cout << "��tree�ļ�ʧ��" << endl;
            return;
        }
        root.readBack(ifs);
        ifs.close();

        cout << "�����ļ���ȡ�ɹ�" << endl;
        showTree();
    }

    // ����д�س���
    void exit()
    {
        char c;
        cout << "�����˳������Ƿ�д�������ļ�(Y/N)��";
        cin >> c;
        if (c == 'N' || c == 'n')
        {
            cout << "����д�������ļ�" << endl;
            return;
        }

        int size;
        ofstream ofs;

        ofs.open("info.txt", ios::out | ios::trunc);
        if (!ofs.is_open())
        {
            cout << "��info�ļ�ʧ��" << endl;
            return;
        }
        ofs << space.size() << " " << memory.size() << " " << blockSize << endl;
        ofs << password << endl;
        ofs << numOfFiles << endl;
        ofs.close();

        ofs.open("space.txt", ios::out | ios::trunc);
        if (!ofs.is_open())
        {
            cout << "��space�ļ�ʧ��" << endl;
            return;
        }
        size = space.size(); // ���ռ�
        for (int i = 0; i < size; i++)
        {
            ofs << space[i] << "\t";
            if (i % 8 == 7)
            {
                ofs << endl;
            }
        }
        ofs.close();

        ofs.open("spaceBlocks.txt", ios::out | ios::trunc);
        if (!ofs.is_open())
        {
            cout << "��spaceBlocks�ļ�ʧ��" << endl;
            return;
        }

        size = spaceBlocks.free.size(); // �����п�
        ofs << size << endl;
        for (int i = 0; i < size; i++)
        {
            ofs << spaceBlocks.free[i] << " ";
        }
        ofs << endl;

        size = spaceBlocks.num; // ����
        for (int i = 0; i < size; i++)
        {
            ofs << spaceBlocks.blocks[i].start << " " << spaceBlocks.blocks[i].used << endl;
        }
        ofs.close();

        ofs.open("tree.txt", ios::out | ios::trunc); // ��״Ŀ¼
        if (!ofs.is_open())
        {
            cout << "��tree�ļ�ʧ��" << endl;
            return;
        }
        root.writeBack(ofs);
        ofs.close();

        cout << "�ѽ��������д�������ļ�" << endl;
    }

    // ��������
    void end()
    {
        int size = memFiles.size();
        if (size)
        {
            showOpenFile();
            cout << "����" << size << "���ļ�δ�رգ��Ƿ�ȷ���˳�(Y/M)��";
            char c;
            cin >> c;
            if (c == 'N' || c == 'n')
            {
                return;
            }
        }
        exit();
    }

    // �����ļ���
    bool createFolder(string name, int protection)
    {
        if (ifIllegal(1))
        {
            return false;
        }

        // �ж��ļ��������Ƿ��ظ�
        if (curFolder->findFolder(name) != -1)
        {
            cout << "�����ļ���ʧ�ܣ��ļ����Ѵ���" << endl;
            return false;
        }
        Folder newFolder(curPath, name, protection);
        curFolder->folders.push_back(newFolder);

        cout << "�����ļ��гɹ���" << curPath << "/" << name << endl;

        return true;
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

                if (ifIllegal(curFolder->protection)) // �ж�Ȩ��
                {
                    curFolder = tmpFolder; // �����ļ���ָ��
                    return false;
                }

                name = "";
            }
        }

        curPath = tmpPath; // ����·��

        return true;
    }

    // �����ļ���ָ���ļ�������ʶ�������͡�λ�á���С���������ԣ�1.����ռ䣬2.����Ŀ¼��
    bool createFile(string name, int type, int protection)
    {
        if (ifIllegal(1))
        {
            return false;
        }

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

        cout << "�����ļ��ɹ���" << curPath << "/" << name << "(" << numOfFiles << ")  ";
        cout << ctime(&newFile.updatetime) << endl;

        numOfFiles++;

        return true;
    }

    // ���ļ���1.��Ŀ¼��Ϣ�����ڴ���ļ����У��������û����̺��ļ�֮�����ϵ��
    bool openFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "���ļ�ʧ�ܣ�û���ҵ��ļ�" << endl;
            return false;
        }
        curFile = &curFolder->files[index];
        int id = curFile->id;

        // ����ļ��Ƿ��Ѿ���
        int size = memFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (memFiles[i].file->id == id)
            {
                cout << "���ļ�ʧ�ܣ��ļ��Ѵ򿪹�" << endl;
                return false;
            }
        }

        memFiles.push_back(MemFile(curFile, -1)); // ��Ŀ¼��Ϣ�����ڴ���ļ���
        cout << "���ļ��ɹ���" << curPath << "/" << name << "(" << id << ")" << endl;

        return true;
    }

    // ��ʾ���ļ���
    void showOpenFile()
    {
        int size = memFiles.size();
        if (size == 0)
        {
            cout << "��ǰû�д򿪵��ļ�" << endl;
            return;
        }

        cout << "��ǰ�򿪵��ļ���" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << i << ": " << memFiles[i].file->name
                 << "(" << memFiles[i].file->id << ")" << endl;
        }
    }

    // �Ӵ��ļ�����ѡ���ļ�
    int getFile(string name) // ͨ���ļ�����ȡ�ļ����
    {
        // ���ڴ���ļ������ҵ�Ŀ���ļ���Ŀ¼��
        vector<int> indexes;
        int num = 0; // Ŀ���ļ��ڴ��ļ����е����

        int size = memFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (memFiles[i].file->name == name)
            {
                indexes.push_back(i);
            }
        }

        // �ж��Ƿ��ҵ�Ŀ���ļ�������ҵ��˶��ͬ���ļ������ѡ��
        if (indexes.size() == 0)
        {
            cout << "ʧ�ܣ�δ���Ѵ��ļ����ҵ����ļ�" << endl;
            return -1;
        }
        else if (indexes.size() > 1)
        {
            cout << "�ҵ����ͬ���ļ���" << name << endl;
            for (int i = 0; i < indexes.size(); i++)
            {
                cout << i << ": " << memFiles[indexes[i]].file->id << " ";
                cout << ctime(&memFiles[indexes[i]].file->updatetime) << endl;
            }
            cout << "������Ŀ���ļ���ţ�" << endl;
            cin >> num;
            if (num < 0 || num >= indexes.size())
            {
                cout << "�������" << endl;
                return -1;
            }
        }

        num = indexes[num];
        curFile = memFiles[num].file;

        if (ifIllegal(curFile->protection)) // �ж�Ȩ��
        {
            return -1;
        }

        tmpFile = &memFiles[num];

        return num;
    }

    int getFile(int id) // ͨ���ļ�ʶ��Ż�ȡ�ļ����
    {
        int size = memFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (memFiles[i].file->id == id)
            {
                return i;
            }
        }
        return -1;
    }

    // ��ʾ�ڴ����ļ�������
    string showMemFile()
    {
        string stmp = "";
        int memLoc = tmpFile->memLoc;
        if (memLoc == -1)
        {
            cout << "���ڴ��ļ�δ����" << endl;
            return stmp;
        }

        cout << "�ļ�����" << tmpFile->file->name << endl;
        int size = tmpFile->size;
        for (int i = 0; i < size; i++)
        {
            int used = memBlocks.blocks[memLoc].used;     // �ڴ���ѷ���ռ�
            int memAddr = memBlocks.blocks[memLoc].start; // �ڴ��ַ
            cout << i << "��";
            for (int j = 0; j < used - 1; j++)
            {
                stmp += (char)memory[memAddr];
                cout << (char)memory[memAddr];
                memAddr++;
            }
            if (0 < used && used < blockSize)
            {
                stmp += (char)memory[memAddr];
                cout << (char)memory[memAddr] << endl;
            }
            cout << endl;
            memLoc = memory[memAddr]; // ��ȡ��һ���ڴ���
        }

        cout << stmp << endl;
        return stmp;
    }

    // ���ļ���ָ���ļ�����Ҫ�����ļ�����ڴ�λ�ã�1.��Ŀ¼��������2.���ļ����ݶ����ڴ�
    bool readFile(string name)
    {
        if (getFile(name) == -1)
        {
            return false;
        }

        if (tmpFile->memLoc != -1) // �ļ��Ѿ���
        {
            showMemFile();
            return true;
        }

        // ��ȡ�ļ���Ϣ
        int size = curFile->size;         // �ļ���С(����)
        int location = curFile->location; // �����

        // �ж��ڴ�ռ����
        if (size > memBlocks.num)
        {
            cout << "���ļ�ʧ�ܣ��ļ������ڴ����޲���" << endl;
            return false;
        }
        if (size > memBlocks.free.size())
        {
            cout << "���ļ�ʧ�ܣ��ڴ�ʣ��ռ䲻�㣬��رղ����ļ����ͷſռ�" << endl;
            return false;
        }

        cout << "�ڴ�ռ���㣬���ڶ�ȡ�ļ����ݵ��ڴ棺" << endl;

        // �����ڴ�
        int memLoc = memBlocks.findFreeBlock(); // ȡ�����п�
        tmpFile->memLoc = memLoc;

        if (size == 0)
        {
            cout << "�ļ�Ϊ��" << endl;
        }

        for (int i = 0; i < size; i++)
        {
            int addr = spaceBlocks.blocks[location].start; // ����ַ
            int used = spaceBlocks.blocks[location].used;  // �����ѷ���ռ�
            int memAddr = memBlocks.blocks[memLoc].start;  // �ڴ��ַ

            memBlocks.blocks[memLoc].used = used; // �����ڴ���ѷ���ռ�

            for (int j = 0; j < used - 1; j++)
            {
                memory[memAddr++] = space[addr++];
            }
            if (0 < used && used < blockSize)
            {
                memory[memAddr] = space[addr];
            }

            if (used == blockSize) // ����������Ҫ�ڿ�ĩ�洢��һ����Ŀ��
            {
                memLoc = memBlocks.findFreeBlock(); // ȡ�����п�
                memory[memAddr] = memLoc;           // ���һλ�洢��һ�����
            }

            location = space[addr]; // ��ȡ��һ�������
        }

        tmpFile->size = size;

        cout << "���ļ��ɹ���" << tmpFile->file->name << "(" << curFile->id << ")" << endl;
        showMemFile();

        return true;
    }

    // д�ļ�����
    bool writeA(int memLoc, string stmp)
    {
        while (memBlocks.blocks[memLoc].used == blockSize) // ���������һ����
        {
            memLoc = memory[memLoc * blockSize + blockSize - 1];
        }
        int memAddr = memBlocks.blocks[memLoc].start + memBlocks.blocks[memLoc].used; // �ڴ��ַ

        for (int i = 0; i < stmp.length(); i++)
        {
            char ctmp = stmp[i];

            if (memBlocks.blocks[memLoc].used == blockSize - 1) // ����������Ҫ�ڿ�ĩ�洢��һ����Ŀ��
            {
                memBlocks.blocks[memLoc].used++;    // ���һλָ����һ�����
                memLoc = memBlocks.findFreeBlock(); // ȡ�����п�
                if (memLoc == -1)
                {
                    cout << "�ڴ�������" << stmp.substr(0, i + 1) << endl;
                    return false;
                }

                memory[memAddr] = memLoc;                 // ��¼��һ�����
                memAddr = memBlocks.blocks[memLoc].start; // ���µ�ַ����һ������
                tmpFile->size++;                          // �����ļ���С
            }

            int data = ctmp;
            memory[memAddr++] = data;        // ��¼����
            memBlocks.blocks[memLoc].used++; // ���¿��ѷ���ռ�
        }
        return true;
    }

    // д�ļ���ɾ
    bool writeD(int memLoc, int data)
    {
        while (data > 0)
        {
            memLoc = tmpFile->memLoc;
            int tmpLoc = memLoc; // ��ʱ�����һ����Ŀ��

            while (memBlocks.blocks[memLoc].used == blockSize) // ���������һ����
            {
                tmpLoc = memLoc;
                memLoc = memory[memLoc * blockSize + blockSize - 1];
            }

            if (memBlocks.blocks[memLoc].used > data) // ��С�㹻����ֱ�Ӽ�
            {
                memBlocks.blocks[memLoc].used -= data;
                data = 0;
            }
            else // �����ͼ���֮����һ����
            {
                if (tmpLoc == memLoc) // ���������һ���黹����
                {
                    cout << "���볤�ȳ����ļ����ȣ���ȫ��ɾ��" << endl;
                    data = 0;
                    memBlocks.blocks[memLoc].used = 0; // �ѷ���ռ���0������Ϊ��ʼ�鲻���ͷ�
                    break;
                }
                data -= memBlocks.blocks[memLoc].used;
                memBlocks.freeBlock(memLoc);

                memLoc = tmpLoc;                 // ���¿��Ϊ��һ����
                memBlocks.blocks[memLoc].used--; // ������һ����ķ���ռ�
                tmpFile->size--;                 // �����ļ���С
            }
        }
        return true;
    }

    // д�ļ�����
    bool writeC(int memLoc, int line, int column, string stmp)
    {
        string orig = showMemFile();
        int len = orig.length();
        int start = line * (blockSize - 1) + column;
        int end = start + stmp.length();

        string newstr = orig.substr(0, start);
        newstr += stmp;
        if (end < len) // ���Ҫд����ַ�������С��ԭ��ʣ�µĳ��ȣ����ȡʣ�ಿ��
        {
            newstr += orig.substr(end, len);
        }

        writeD(memLoc, len);
        writeA(memLoc, newstr);

        return true;
    }

    // д�ļ�����
    bool writeI(int memLoc, int line, int column, string stmp)
    {
        string orig = showMemFile();
        int len = orig.length();
        int start = line * (blockSize - 1) + column;

        string newstr = orig.substr(0, start);
        newstr += stmp;
        newstr += orig.substr(start, len);

        writeD(memLoc, len);
        writeA(memLoc, newstr);

        return true;
    }

    // д�ļ���ָ���ļ�����Ҫд���ļ������ݣ�1.��Ŀ¼��������2.����Ŀ¼���ļ�ָ��д�ļ�
    bool writeFile(string name)
    {
        if (getFile(name) == -1)
        {
            return false;
        }

        if (tmpFile->memLoc == -1)
        {
            cout << "�ļ���δ��ȡ���ڴ���" << endl;
            return false;
        }

        string stmp;
        char ctmp;
        while (1)
        {
            cout << "��ѡ����Ĳ��� 1.����2.ɾ��3.�ģ�4.�� (����Q�˳�) (��֧������!)��";
            getline(cin, stmp);
            ctmp = stmp[0];

            int data = 0;
            int memLoc = tmpFile->memLoc; // ��ȡ�ļ��������λ��
            int line, column;

            switch (ctmp)
            {
            case '1':
                cout << "������Ҫ���ӵ����ݣ�" << endl;
                getline(cin, stmp);
                writeA(memLoc, stmp);
                break;
            case '2':
                if (memBlocks.blocks[memLoc].used == 0)
                {
                    cout << "�ļ��ѿ�" << endl;
                    break;
                    ;
                }
                cout << "������Ҫɾ�����ȣ�" << endl;
                cin >> stmp;
                cin.ignore();
                for (int i = 0; i < stmp.length(); i++)
                {
                    ctmp = stmp[i];
                    if (ctmp < '0' || ctmp > '9')
                    {
                        cout << "�����������������룺" << stmp.substr(0, i + 1) << endl;
                        data = -1;
                    }
                    data = data * 10 + ctmp - '0';
                }
                if (data == -1)
                {
                    continue;
                }
                writeD(memLoc, data);
                break;
            case '3':
                cout << "������Ҫ�޸ĵ�������������";
                cin >> stmp;
                line = stmp[0] - '0';
                if (line < 0 || line > tmpFile->size)
                {
                    cout << "���������������ļ����ȣ�����������" << endl;
                    continue;
                }
                cin >> stmp;
                column = stmp[0] - '0';
                if (column < 0 || column > memBlocks.blocks[memLoc].used)
                {
                    cout << "���������������ļ����ȣ�����������" << endl;
                }
                cout << "������Ҫ�޸ĵ����ݣ�";
                cin.ignore();
                getline(cin, stmp);
                writeC(memLoc, line, column, stmp);
                break;
            case '4':
                cout << "������Ҫ�����������������";
                cin >> stmp;
                line = stmp[0] - '0';
                if (line < 0 || line > tmpFile->size)
                {
                    cout << "���������������ļ����ȣ�����������" << endl;
                    continue;
                }
                cin >> stmp;
                column = stmp[0] - '0';
                if (column < 0 || column > memBlocks.blocks[memLoc].used)
                {
                    cout << "���������������ļ����ȣ�����������" << endl;
                }
                cout << "������Ҫ��������ݣ�";
                cin.ignore();
                getline(cin, stmp);
                writeI(memLoc, line, column, stmp);
                break;
            case 'Q':
            case 'q':
                cout << "�˳�д�ļ�����" << endl;
                tmpFile->dirty = true;
                return true;
            default:
                cout << "�������" << endl;
                break;
            }

            showMemFile();
        }
        cout << "�쳣�Ƴ�" << endl;
        return false;
    }

    // �ر��ļ���1.���������е�Ŀ¼��Ϣ�����ͷ��û����̺��ļ�֮�����ϵ��2.�����Ƿ�д���ļ�
    bool closeFile(string name)
    {
        int num = getFile(name);
        if (num == -1)
        {
            return false;
        }

        int memLoc = tmpFile->memLoc; // ��ȡ�ļ��������λ��

        if (tmpFile->dirty)
        {
            cout << "�ļ��иĶ����Ƿ񱣴�(Y/N)��";
            string stmp;
            getline(cin, stmp);
            if (stmp[0] == 'Y' || stmp[0] == 'y')
            {

                int size = tmpFile->size; // ��ȡ���º���ļ���С
                if (size > (curFile->size + spaceBlocks.free.size()))
                {
                    cout << "���ռ䲻�㣬�޷�д�أ����������ɾ�㶫��" << endl;
                    return false;
                }

                int location = curFile->location;        // ��ȡ�ļ��ڴ����е�λ��
                while (!spaceBlocks.freeBlock(location)) // ����Ҳ��Ҫȫ����дһ�飬����������װ�����ܾ������
                {
                    location = space[location * blockSize + blockSize - 1];
                }

                location = spaceBlocks.findFreeBlock(); // ȡ�����п�
                curFile->location = location;

                for (int i = 0; i < size; i++)
                {
                    int addr = spaceBlocks.blocks[location].start; // ����ַ
                    int used = memBlocks.blocks[memLoc].used;      // �ڴ���ѷ���ռ�
                    int memAddr = memBlocks.blocks[memLoc].start;  // �ڴ��ַ

                    spaceBlocks.blocks[location].used = used; // �����ѷ���ռ�

                    for (int j = 0; j < used - 1; j++)
                    {
                        space[addr++] = memory[memAddr++]; // �洢����
                    }
                    if (0 < used && used < blockSize)
                    {
                        space[addr] = memory[memAddr];
                    }

                    if (used == blockSize) // ����������Ҫ�ڿ�ĩ�洢��һ����Ŀ��
                    {
                        location = spaceBlocks.findFreeBlock(); // ȡ�����п�
                        space[addr] = location;                 // ���һλ�洢��һ�����
                    }

                    memLoc = memory[memAddr]; // ��ȡ��һ���ڴ���
                }

                curFile->size = size;             // �����ļ���С
                curFile->updatetime = time(NULL); // �����޸�ʱ��

                cout << "д���ļ��ɹ�" << endl;
            }
        }

        // �����ڴ��
        memLoc = tmpFile->memLoc;
        while (!memBlocks.freeBlock(memLoc))
        {
            memLoc = memory[memLoc * blockSize + blockSize - 1];
        }

        cout << "�ر��ļ��ɹ���" << name << "(" << curFile->id << ")" << endl;

        // �Ӵ��ļ�����ɾ��
        memFiles.erase(memFiles.begin() + num);

        return true;
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
        curFile = &curFolder->files[index];

        if (ifIllegal(curFile->protection))
        {
            return false;
        }

        if (getFile(curFile->id) != -1)
        {
            cout << "ɾ���ļ�ʧ�ܣ��ļ�δ�ر�" << endl;
            return false;
        }

        // �ͷſռ�
        int location = curFile->location;
        while (!spaceBlocks.freeBlock(location))
        {
            location = space[location * blockSize + blockSize - 1];
        }

        cout << "ɾ���ļ��ɹ���" << curPath << "/" << name << "(" << curFile->id << ")" << endl;

        // ɾ��Ŀ¼��
        curFolder->files.erase(curFolder->files.begin() + index);

        return true;
    }
};