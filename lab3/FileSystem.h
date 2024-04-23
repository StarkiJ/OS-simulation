#include "Block-Blocks-File-Folder.h"

class FileSystem
{
    string password; // 密码
    int authority;   // 权限级别

    int blockSize;  // 块大小
    int numOfFiles; // 历史文件数量(包含删除文件)

    vector<int> space;  // 外存空间
    vector<int> memory; // 内存

    vector<MemFile> memFiles; // 内存文件集合

    Blocks spaceBlocks; // 外存空间块
    Blocks memBlocks;   // 内存块

    Folder root;       // 根目录
    Folder *curFolder; // 当前文件夹指针
    string curPath;    // 当前路径

    File *curFile;    // 当前文件指针
    MemFile *tmpFile; // 当前内存文件指针

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

    // 设置密码
    void setPassword()
    {
        cout << "请设置密码：";
        getline(cin, password);
    }

    // 管理员界面
    void adjustAuthority(string word)
    {
        if (word != password)
        {
            cout << "密码错误" << endl;
            return;
        }

        cout << "0.游客权限" << endl;
        cout << "1.用户权限" << endl;
        cout << "2.管理员权限" << endl;
        cout << "3.修改密码" << endl;
        cout << "请选择权限级别：";
        int itmp;
        while (1)
        {
            cin >> itmp;
            cin.ignore();
            if (itmp < 0 || itmp > 3)
            {
                cout << "输入错误，请重新输入！" << endl;
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

    // 判断是否违规
    bool ifIllegal(int limit)
    {
        if (authority < limit)
        {
            cout << "权限不足" << endl;
            return true;
        }
        return false;
    }

    // 显示当前路径
    void showPath()
    {
        cout << "当前路径：" << curPath << endl;
    }

    // 显示当前文件夹内容
    void showThisFolder()
    {
        curFolder->showFolder();
    }

    // 显示树状目录
    void showTree()
    {
        root.showAll();
    }

    // 开始读取程序
    void start()
    {
        int size = 0;
        int itmp;
        string stmp;
        ifstream ifs;

        ifs.open("info.txt", ios::in);
        if (ifs.is_open())
        {
            // 空间大小不符则数据文件无效
            if ((!(ifs >> size) || size != space.size()) ||
                (!(ifs >> size) || size != memory.size()) ||
                (!(ifs >> size) || size != blockSize))
            {
                cout << "数据文件无效" << endl;
                return;
            }

            cout << "数据文件有效，是否读取(Y/N)：";
            cin >> stmp;
            if (stmp == "N" || stmp == "n")
            {
                ifs.close();
                cout << "放弃读取数据文件，新建文件系统" << endl;
                return;
            }

            ifs.ignore();
            getline(ifs, password);
            ifs >> numOfFiles;
        }
        else
        {
            cout << "未找到info文件" << endl;
            return;
        }
        ifs.close();

        ifs.open("space.txt", ios::in);
        if (!ifs.is_open())
        {
            cout << "打开space文件失败" << endl;
            return;
        }
        size = space.size(); // 读取外存空间
        for (int i = 0; i < size; i++)
        {
            ifs >> itmp;
            space[i] = itmp;
        }
        ifs.close();

        ifs.open("spaceBlocks.txt", ios::in);
        if (!ifs.is_open())
        {
            cout << "打开spaceBlocks文件失败" << endl;
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
            cout << "打开tree文件失败" << endl;
            return;
        }
        root.readBack(ifs);
        ifs.close();

        cout << "数据文件读取成功" << endl;
        showTree();
    }

    // 结束写回程序
    void exit()
    {
        char c;
        cout << "正在退出程序，是否写回数据文件(Y/N)：";
        cin >> c;
        if (c == 'N' || c == 'n')
        {
            cout << "放弃写回数据文件" << endl;
            return;
        }

        int size;
        ofstream ofs;

        ofs.open("info.txt", ios::out | ios::trunc);
        if (!ofs.is_open())
        {
            cout << "打开info文件失败" << endl;
            return;
        }
        ofs << space.size() << " " << memory.size() << " " << blockSize << endl;
        ofs << password << endl;
        ofs << numOfFiles << endl;
        ofs.close();

        ofs.open("space.txt", ios::out | ios::trunc);
        if (!ofs.is_open())
        {
            cout << "打开space文件失败" << endl;
            return;
        }
        size = space.size(); // 外存空间
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
            cout << "打开spaceBlocks文件失败" << endl;
            return;
        }

        size = spaceBlocks.free.size(); // 外存空闲块
        ofs << size << endl;
        for (int i = 0; i < size; i++)
        {
            ofs << spaceBlocks.free[i] << " ";
        }
        ofs << endl;

        size = spaceBlocks.num; // 外存块
        for (int i = 0; i < size; i++)
        {
            ofs << spaceBlocks.blocks[i].start << " " << spaceBlocks.blocks[i].used << endl;
        }
        ofs.close();

        ofs.open("tree.txt", ios::out | ios::trunc); // 树状目录
        if (!ofs.is_open())
        {
            cout << "打开tree文件失败" << endl;
            return;
        }
        root.writeBack(ofs);
        ofs.close();

        cout << "已将外存数据写回数据文件" << endl;
    }

    // 结束程序
    void end()
    {
        int size = memFiles.size();
        if (size)
        {
            showOpenFile();
            cout << "还有" << size << "个文件未关闭，是否确定退出(Y/M)：";
            char c;
            cin >> c;
            if (c == 'N' || c == 'n')
            {
                return;
            }
        }
        exit();
    }

    // 创建文件夹
    bool createFolder(string name, int protection)
    {
        if (ifIllegal(1))
        {
            return false;
        }

        // 判断文件夹名字是否重复
        if (curFolder->findFolder(name) != -1)
        {
            cout << "创建文件夹失败，文件夹已存在" << endl;
            return false;
        }
        Folder newFolder(curPath, name, protection);
        curFolder->folders.push_back(newFolder);

        cout << "创建文件夹成功：" << curPath << "/" << name << endl;

        return true;
    }

    // 打开文件夹
    bool openFolder(string path)
    {
        string name = "";              // 文件夹的名字
        string tmpPath = curPath;      // 临时路径
        Folder *tmpFolder = curFolder; // 临时文件夹指针
        int len = path.length();       // 路径长度

        // 防止输入路径中带有"root:"
        if (path.substr(0, 5) == "root:") // 防止输入路径中带有"root:"
        {
            if (len == 5)
            {
                path += "/";
            }
            path = path.substr(5, len);
            len = path.length();
        }

        // 判断路径格式
        if (path[0] == '/') // 绝对路径
        {
            if (path == "/") // 特殊情况，防止"/"叠加
            {
                tmpPath = "root:";
            }
            else
            {
                tmpPath = "root:" + path;
            }
            curFolder = &root;
        }
        else if (path.substr(0, 2) == "./") // 相对路径：当前目录
        {
            if (path != "./")
            {
                tmpPath += path.substr(1, len);
            }
            else
            {
                cout << "??? 原地TP ???" << endl;
                return false;
            }
        }
        else if (path.substr(0, 3) == "../") // 相对路径：上一级目录
        {
            if (curPath == "root:")
            {
                cout << "打开文件夹失败，根目录没有父文件夹" << endl;
                return false;
            }

            int curlen = curPath.length(); // 当前路径长度
            for (int i = curlen; i > 0; i--)
            {
                if (curPath[i] == '/')
                {
                    if (!openFolder(curPath.substr(0, i)))
                    {
                        cout << "打开文件夹失败，父文件夹路径错误" << endl;
                        return false;
                    }

                    if (path == "../") // 特殊情况
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
            cout << "打开文件夹失败，路径输入错误！" << endl;
            return false;
        }

        // 遍历路径，寻找目标文件夹
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
                i++; // 防止最后一位无法输出
            }

            if (name != "")
            {
                int index = curFolder->findFolder(name);
                if (index == -1)
                {
                    cout << "打开文件夹失败，没有找到文件夹: " << path.substr(0, i) << endl;
                    curFolder = tmpFolder; // 回退文件夹指针
                    return false;
                }
                curFolder = &curFolder->folders[index];

                if (ifIllegal(curFolder->protection)) // 判断权限
                {
                    curFolder = tmpFolder; // 回退文件夹指针
                    return false;
                }

                name = "";
            }
        }

        curPath = tmpPath; // 更新路径

        return true;
    }

    // 创建文件，指明文件名、标识符、类型、位置、大小、保护属性：1.分配空间，2.建立目录项
    bool createFile(string name, int type, int protection)
    {
        if (ifIllegal(1))
        {
            return false;
        }

        // 判断文件名是否重复
        if (curFolder->findFile(name) != -1)
        {
            cout << "创建文件失败，文件已存在" << endl;
            return false;
        }

        // 判断空间是否足够
        int location = spaceBlocks.findFreeBlock();
        if (location == -1)
        {
            cout << "创建文件失败，空间不足" << endl;
            return false;
        }

        File newFile(name, numOfFiles, type, location, protection);
        curFolder->files.push_back(newFile);

        cout << "创建文件成功：" << curPath << "/" << name << "(" << numOfFiles << ")  ";
        cout << ctime(&newFile.updatetime) << endl;

        numOfFiles++;

        return true;
    }

    // 打开文件：1.将目录信息读入内存打开文件表中，建立起用户进程和文件之间的联系。
    bool openFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "打开文件失败，没有找到文件" << endl;
            return false;
        }
        curFile = &curFolder->files[index];
        int id = curFile->id;

        // 检测文件是否已经打开
        int size = memFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (memFiles[i].file->id == id)
            {
                cout << "打开文件失败，文件已打开过" << endl;
                return false;
            }
        }

        memFiles.push_back(MemFile(curFile, -1)); // 将目录信息读入内存打开文件表
        cout << "打开文件成功：" << curPath << "/" << name << "(" << id << ")" << endl;

        return true;
    }

    // 显示打开文件表
    void showOpenFile()
    {
        int size = memFiles.size();
        if (size == 0)
        {
            cout << "当前没有打开的文件" << endl;
            return;
        }

        cout << "当前打开的文件：" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << i << ": " << memFiles[i].file->name
                 << "(" << memFiles[i].file->id << ")" << endl;
        }
    }

    // 从打开文件表中选择文件
    int getFile(string name) // 通过文件名获取文件编号
    {
        // 从内存打开文件表中找到目标文件的目录项
        vector<int> indexes;
        int num = 0; // 目标文件在打开文件表中的序号

        int size = memFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (memFiles[i].file->name == name)
            {
                indexes.push_back(i);
            }
        }

        // 判断是否找到目标文件，如果找到了多个同名文件则进行选择
        if (indexes.size() == 0)
        {
            cout << "失败，未在已打开文件中找到该文件" << endl;
            return -1;
        }
        else if (indexes.size() > 1)
        {
            cout << "找到多个同名文件：" << name << endl;
            for (int i = 0; i < indexes.size(); i++)
            {
                cout << i << ": " << memFiles[indexes[i]].file->id << " ";
                cout << ctime(&memFiles[indexes[i]].file->updatetime) << endl;
            }
            cout << "请输入目标文件编号：" << endl;
            cin >> num;
            if (num < 0 || num >= indexes.size())
            {
                cout << "输入错误" << endl;
                return -1;
            }
        }

        num = indexes[num];
        curFile = memFiles[num].file;

        if (ifIllegal(curFile->protection)) // 判断权限
        {
            return -1;
        }

        tmpFile = &memFiles[num];

        return num;
    }

    int getFile(int id) // 通过文件识别号获取文件编号
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

    // 显示内存中文件的内容
    string showMemFile()
    {
        string stmp = "";
        int memLoc = tmpFile->memLoc;
        if (memLoc == -1)
        {
            cout << "该内存文件未分配" << endl;
            return stmp;
        }

        cout << "文件名：" << tmpFile->file->name << endl;
        int size = tmpFile->size;
        for (int i = 0; i < size; i++)
        {
            int used = memBlocks.blocks[memLoc].used;     // 内存块已分配空间
            int memAddr = memBlocks.blocks[memLoc].start; // 内存地址
            cout << i << "：";
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
            memLoc = memory[memAddr]; // 获取下一个内存块号
        }

        cout << stmp << endl;
        return stmp;
    }

    // 读文件，指明文件名和要读入文件块的内存位置：1.在目录中搜索，2.将文件内容读入内存
    bool readFile(string name)
    {
        if (getFile(name) == -1)
        {
            return false;
        }

        if (tmpFile->memLoc != -1) // 文件已经打开
        {
            showMemFile();
            return true;
        }

        // 读取文件信息
        int size = curFile->size;         // 文件大小(块数)
        int location = curFile->location; // 外存块号

        // 判断内存空间情况
        if (size > memBlocks.num)
        {
            cout << "读文件失败，文件过大，内存上限不足" << endl;
            return false;
        }
        if (size > memBlocks.free.size())
        {
            cout << "读文件失败，内存剩余空间不足，请关闭部分文件以释放空间" << endl;
            return false;
        }

        cout << "内存空间充足，正在读取文件数据到内存：" << endl;

        // 读到内存
        int memLoc = memBlocks.findFreeBlock(); // 取出空闲块
        tmpFile->memLoc = memLoc;

        if (size == 0)
        {
            cout << "文件为空" << endl;
        }

        for (int i = 0; i < size; i++)
        {
            int addr = spaceBlocks.blocks[location].start; // 外存地址
            int used = spaceBlocks.blocks[location].used;  // 外存块已分配空间
            int memAddr = memBlocks.blocks[memLoc].start;  // 内存地址

            memBlocks.blocks[memLoc].used = used; // 更新内存块已分配空间

            for (int j = 0; j < used - 1; j++)
            {
                memory[memAddr++] = space[addr++];
            }
            if (0 < used && used < blockSize)
            {
                memory[memAddr] = space[addr];
            }

            if (used == blockSize) // 块已满则需要在块末存储下一个块的块号
            {
                memLoc = memBlocks.findFreeBlock(); // 取出空闲块
                memory[memAddr] = memLoc;           // 最后一位存储下一个块号
            }

            location = space[addr]; // 获取下一个外存块号
        }

        tmpFile->size = size;

        cout << "读文件成功：" << tmpFile->file->name << "(" << curFile->id << ")" << endl;
        showMemFile();

        return true;
    }

    // 写文件：增
    bool writeA(int memLoc, string stmp)
    {
        while (memBlocks.blocks[memLoc].used == blockSize) // 遍历到最后一个块
        {
            memLoc = memory[memLoc * blockSize + blockSize - 1];
        }
        int memAddr = memBlocks.blocks[memLoc].start + memBlocks.blocks[memLoc].used; // 内存地址

        for (int i = 0; i < stmp.length(); i++)
        {
            char ctmp = stmp[i];

            if (memBlocks.blocks[memLoc].used == blockSize - 1) // 块已满则需要在块末存储下一个块的块号
            {
                memBlocks.blocks[memLoc].used++;    // 最后一位指向下一个块号
                memLoc = memBlocks.findFreeBlock(); // 取出空闲块
                if (memLoc == -1)
                {
                    cout << "内存已满：" << stmp.substr(0, i + 1) << endl;
                    return false;
                }

                memory[memAddr] = memLoc;                 // 记录下一个块号
                memAddr = memBlocks.blocks[memLoc].start; // 更新地址到下一个块首
                tmpFile->size++;                          // 更新文件大小
            }

            int data = ctmp;
            memory[memAddr++] = data;        // 记录输入
            memBlocks.blocks[memLoc].used++; // 更新块已分配空间
        }
        return true;
    }

    // 写文件：删
    bool writeD(int memLoc, int data)
    {
        while (data > 0)
        {
            memLoc = tmpFile->memLoc;
            int tmpLoc = memLoc; // 临时存放上一个块的块号

            while (memBlocks.blocks[memLoc].used == blockSize) // 遍历到最后一个块
            {
                tmpLoc = memLoc;
                memLoc = memory[memLoc * blockSize + blockSize - 1];
            }

            if (memBlocks.blocks[memLoc].used > data) // 大小足够减则直接减
            {
                memBlocks.blocks[memLoc].used -= data;
                data = 0;
            }
            else // 不够就减完之后到上一个块
            {
                if (tmpLoc == memLoc) // 如果减到第一个块还不够
                {
                    cout << "输入长度超出文件长度，将全部删除" << endl;
                    data = 0;
                    memBlocks.blocks[memLoc].used = 0; // 已分配空间置0，但作为起始块不能释放
                    break;
                }
                data -= memBlocks.blocks[memLoc].used;
                memBlocks.freeBlock(memLoc);

                memLoc = tmpLoc;                 // 更新块号为上一个块
                memBlocks.blocks[memLoc].used--; // 更新上一个块的分配空间
                tmpFile->size--;                 // 更新文件大小
            }
        }
        return true;
    }

    // 写文件：改
    bool writeC(int memLoc, int line, int column, string stmp)
    {
        string orig = showMemFile();
        int len = orig.length();
        int start = line * (blockSize - 1) + column;
        int end = start + stmp.length();

        string newstr = orig.substr(0, start);
        newstr += stmp;
        if (end < len) // 如果要写入的字符串长度小于原文剩下的长度，则截取剩余部分
        {
            newstr += orig.substr(end, len);
        }

        writeD(memLoc, len);
        writeA(memLoc, newstr);

        return true;
    }

    // 写文件：插
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

    // 写文件，指明文件名和要写入文件的内容：1.在目录中搜索，2.利用目录的文件指针写文件
    bool writeFile(string name)
    {
        if (getFile(name) == -1)
        {
            return false;
        }

        if (tmpFile->memLoc == -1)
        {
            cout << "文件还未读取到内存中" << endl;
            return false;
        }

        string stmp;
        char ctmp;
        while (1)
        {
            cout << "请选择你的操作 1.增，2.删，3.改，4.插 (输入Q退出) (不支持中文!)：";
            getline(cin, stmp);
            ctmp = stmp[0];

            int data = 0;
            int memLoc = tmpFile->memLoc; // 获取文件在主存的位置
            int line, column;

            switch (ctmp)
            {
            case '1':
                cout << "请输入要增加的内容：" << endl;
                getline(cin, stmp);
                writeA(memLoc, stmp);
                break;
            case '2':
                if (memBlocks.blocks[memLoc].used == 0)
                {
                    cout << "文件已空" << endl;
                    break;
                    ;
                }
                cout << "请输入要删除长度：" << endl;
                cin >> stmp;
                cin.ignore();
                for (int i = 0; i < stmp.length(); i++)
                {
                    ctmp = stmp[i];
                    if (ctmp < '0' || ctmp > '9')
                    {
                        cout << "输入有误，请重新输入：" << stmp.substr(0, i + 1) << endl;
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
                cout << "请输入要修改的行数和列数：";
                cin >> stmp;
                line = stmp[0] - '0';
                if (line < 0 || line > tmpFile->size)
                {
                    cout << "输入行数不符合文件长度，请重新输入" << endl;
                    continue;
                }
                cin >> stmp;
                column = stmp[0] - '0';
                if (column < 0 || column > memBlocks.blocks[memLoc].used)
                {
                    cout << "输入列数不符合文件长度，请重新输入" << endl;
                }
                cout << "请输入要修改的内容：";
                cin.ignore();
                getline(cin, stmp);
                writeC(memLoc, line, column, stmp);
                break;
            case '4':
                cout << "请输入要插入的行数和列数：";
                cin >> stmp;
                line = stmp[0] - '0';
                if (line < 0 || line > tmpFile->size)
                {
                    cout << "输入行数不符合文件长度，请重新输入" << endl;
                    continue;
                }
                cin >> stmp;
                column = stmp[0] - '0';
                if (column < 0 || column > memBlocks.blocks[memLoc].used)
                {
                    cout << "输入列数不符合文件长度，请重新输入" << endl;
                }
                cout << "请输入要插入的内容：";
                cin.ignore();
                getline(cin, stmp);
                writeI(memLoc, line, column, stmp);
                break;
            case 'Q':
            case 'q':
                cout << "退出写文件程序" << endl;
                tmpFile->dirty = true;
                return true;
            default:
                cout << "输入错误" << endl;
                break;
            }

            showMemFile();
        }
        cout << "异常推出" << endl;
        return false;
    }

    // 关闭文件：1.撤销主存中的目录信息，并释放用户进程和文件之间的联系，2.考虑是否写回文件
    bool closeFile(string name)
    {
        int num = getFile(name);
        if (num == -1)
        {
            return false;
        }

        int memLoc = tmpFile->memLoc; // 获取文件在主存的位置

        if (tmpFile->dirty)
        {
            cout << "文件有改动，是否保存(Y/N)：";
            string stmp;
            getline(cin, stmp);
            if (stmp[0] == 'Y' || stmp[0] == 'y')
            {

                int size = tmpFile->size; // 获取更新后的文件大小
                if (size > (curFile->size + spaceBlocks.free.size()))
                {
                    cout << "外存空间不足，无法写回，你可以试着删点东西" << endl;
                    return false;
                }

                int location = curFile->location;        // 获取文件在磁盘中的位置
                while (!spaceBlocks.freeBlock(location)) // 反正也是要全部重写一遍，不如整个重装，还能均衡损耗
                {
                    location = space[location * blockSize + blockSize - 1];
                }

                location = spaceBlocks.findFreeBlock(); // 取出空闲块
                curFile->location = location;

                for (int i = 0; i < size; i++)
                {
                    int addr = spaceBlocks.blocks[location].start; // 外存地址
                    int used = memBlocks.blocks[memLoc].used;      // 内存块已分配空间
                    int memAddr = memBlocks.blocks[memLoc].start;  // 内存地址

                    spaceBlocks.blocks[location].used = used; // 更新已分配空间

                    for (int j = 0; j < used - 1; j++)
                    {
                        space[addr++] = memory[memAddr++]; // 存储数据
                    }
                    if (0 < used && used < blockSize)
                    {
                        space[addr] = memory[memAddr];
                    }

                    if (used == blockSize) // 块已满则需要在块末存储下一个块的块号
                    {
                        location = spaceBlocks.findFreeBlock(); // 取出空闲块
                        space[addr] = location;                 // 最后一位存储下一个块号
                    }

                    memLoc = memory[memAddr]; // 获取下一个内存块号
                }

                curFile->size = size;             // 更新文件大小
                curFile->updatetime = time(NULL); // 更新修改时间

                cout << "写回文件成功" << endl;
            }
        }

        // 清理内存块
        memLoc = tmpFile->memLoc;
        while (!memBlocks.freeBlock(memLoc))
        {
            memLoc = memory[memLoc * blockSize + blockSize - 1];
        }

        cout << "关闭文件成功：" << name << "(" << curFile->id << ")" << endl;

        // 从打开文件表中删除
        memFiles.erase(memFiles.begin() + num);

        return true;
    }

    // 删除文件，指明文件名:1.在目录中搜索，2.释放空间，3.删除目录项
    bool deleteFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "删除文件失败，没有找到文件" << endl;
            return false;
        }
        curFile = &curFolder->files[index];

        if (ifIllegal(curFile->protection))
        {
            return false;
        }

        if (getFile(curFile->id) != -1)
        {
            cout << "删除文件失败，文件未关闭" << endl;
            return false;
        }

        // 释放空间
        int location = curFile->location;
        while (!spaceBlocks.freeBlock(location))
        {
            location = space[location * blockSize + blockSize - 1];
        }

        cout << "删除文件成功：" << curPath << "/" << name << "(" << curFile->id << ")" << endl;

        // 删除目录项
        curFolder->files.erase(curFolder->files.begin() + index);

        return true;
    }
};