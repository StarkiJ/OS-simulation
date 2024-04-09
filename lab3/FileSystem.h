#include "Block-Blocks-File-Folder.h"

class FileSystem
{
    string password; // 密码
    int authority;   // 权限级别

    vector<int> space;  // 外存空间
    vector<int> memory; // 内存

    vector<File *> openFiles; // 内存打开文件表

    Blocks spaceBlocks; // 外存空间块
    Blocks memBlocks;   // 内存块

    int blockSize;  // 块大小
    int numOfFiles; // 历史文件数量(包含删除文件)

    Folder root;       // 根目录
    Folder *curFolder; // 当前文件夹指针
    string curPath;    // 当前路径

    File *curFile; // 当前文件指针

public:
    FileSystem(int sizeOfSpace, int sizeOfMemory, int blockSize)
    {
        password = "admin";
        authority = 1;

        space = vector<int>(sizeOfSpace, -1);
        memory = vector<int>(sizeOfMemory, -1);

        this->blockSize = blockSize;
        numOfFiles = 0;

        spaceBlocks = Blocks(sizeOfSpace, blockSize);
        memBlocks = Blocks(sizeOfMemory, blockSize);

        root = Folder("root", 0);
        curFolder = &root;
        curPath = "root:";
    }

    // 设置密码
    void setPassword()
    {
        getline(cin, password);
    }

    // 调整操作权限
    void adjustAuthority(string word)
    {
        if (word != password)
        {
            cout << "密码错误" << endl;
            return;
        }

        cout << "请选择权限级别：" << endl;
        cout << "0.游客权限" << endl;
        cout << "1.用户权限" << endl;
        cout << "2.管理员权限" << endl;
        cin >> authority;
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
        showPath();

        cout << "文件夹：";
        int size = curFolder->folders.size();
        for (int i = 0; i < size; i++)
        {
            cout << curFolder->folders[i].name << " ";
        }
        cout << endl;

        cout << "文件：";
        size = curFolder->files.size();
        for (int i = 0; i < size; i++)
        {
            cout << curFolder->files[i].name << " ";
        }
        cout << endl;
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
        Folder newFolder(name, protection);
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

        cout << "创建文件成功：" << curPath << "/" << name << "(" << numOfFiles << ") ";
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
        int size = openFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (openFiles[i]->id == id)
            {
                cout << "打开文件失败，文件已打开过" << endl;
                return false;
            }
        }

        openFiles.push_back(curFile); // 将目录信息读入内存打开文件表
        cout << "打开文件成功：" << curPath << "/" << name << "(" << id << ")" << endl;

        return true;
    }

    // 读文件，指明文件名和要读入文件块的内存位置：1.在目录中搜索，2.将文件内容读入内存
    bool readFile(string name)
    {
        // 从内存打开文件表中找到目标文件的目录项
        vector<int> index;
        int size = openFiles.size();
        for (int i = 0; i < size; i++)
        {
            if (openFiles[i]->name == name)
            {
                index.push_back(i);
            }
        }

        // 判断是否找到目标文件，如果找到了多个同名文件则进行选择
        if (index.size() == 0)
        {
            cout << "读文件失败，未在已打开文件中找到该文件" << endl;
            return false;
        }
        else if (index.size() == 1)
        {
            curFile = &curFolder->files[index[0]];
        }
        else
        {
            cout << "找到多个同名文件：" << name << endl;
            for (int i = 0; i < index.size(); i++)
            {
                cout << i << ": " << openFiles[index[i]]->id << " ";
                cout << ctime(&openFiles[index[i]]->updatetime) << endl;
            }
            cout << "请输入要打开的文件编号：" << endl;
            int num;
            cin >> num;
            curFile = &curFolder->files[index[num]];
        }

        if (ifIllegal(curFile->protection)) // 判断权限
        {
            return false;
        }

        // 读取文件信息
        int size = curFile->size;
        int location = curFile->location; // 块号
        for (int i = 0; i < curFile->size; i++)
        {
            int num = spaceBlocks.blocks[location].start; // 外存地址
            int used = spaceBlocks.blocks[location].used;
            for (int j = 0; j < used - 1; j++)
            {
                cout << space[num++] << " ";
            }
            cout << endl;

            location = space[num];
        }
    }

    // 写文件，指明文件名和要写入文件的内容：1.在目录中搜索，2.利用目录的文件指针写文件
    bool writeFile(string name, string data)
    {
        int index = curFolder->findFile(name);
    }

    // 关闭文件：1.撤销主存中的目录信息，并释放用户进程和文件之间的联系，2.考虑是否写回文件
    void closeFile(string name)
    {
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

        // 释放空间
        int location = curFile->location;
        while (!spaceBlocks.freeBlock(location))
        {
            location = space[location * blockSize + blockSize - 1];
        }

        // 删除目录项
        curFolder->files.erase(curFolder->files.begin() + index);
    }
};