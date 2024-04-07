#include "headlab2.h"

using namespace std;

// 块
class Block
{
public:
    int num;   // 序号
    int start; // 起始地址
    int used;  // 已分配

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
            blocks.push_back(Block(i, i * size));
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

        return index;
    }

    // 释放块
    int freeBlock(int index)
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

class FileSystem
{
    string password; // 密码
    int authority;   // 权限级别

    vector<int> space;  // 外存空间
    vector<int> memory; // 内存

    vector<File> openFiles; // 内存打开文件表

    Blocks spaceBlocks; // 外存空间块
    Blocks memBlocks;   // 内存块

    int blockSize;  // 块大小
    int numOfFiles; // 历史文件数量(包含删除文件)

    Folder root;       // 根目录
    Folder *curFolder; // 当前文件夹指针
    string curPath;    // 当前路径

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
        cout << "0.禁止访问" << endl;
        cout << "1.只读" << endl;
        cout << "2.读写" << endl;
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
                name = "";
            }
        }

        curPath = tmpPath; // 更新路径

        return true;
    }

    // 创建文件夹
    bool createFolder(string name)
    {
        // 判断文件夹名字是否重复
        if (curFolder->findFolder(name) != -1)
        {
            cout << "创建文件夹失败，文件夹已存在" << endl;
            return false;
        }
        Folder newFolder(name);
        curFolder->folders.push_back(newFolder);

        cout << "创建文件夹成功：" << curPath << "/" << name << endl;
        return true;
    }

    // 创建文件，指明文件名、标识符、类型、位置、大小、保护属性：1.分配空间，2.建立目录项
    bool createFile(string name, int type, int protection)
    {
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
        numOfFiles++;

        cout << "创建文件成功：" << curPath << "/" << name << endl;
        return true;
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
    bool deleteFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "删除文件失败，没有找到文件" << endl;
            return false;
        }

        // 释放空间
        int location = curFolder->files[index].location;
        while (!spaceBlocks.freeBlock(location))
        {
            location = space[location * blockSize + blockSize - 1];
        }

        // 删除目录项
        curFolder->files.erase(curFolder->files.begin() + index);
    }

    // 打开文件，指明文件名：1.将目录信息读入内存打开文件表中，建立起用户进程和文件之间的联系。
    bool openFile(string name)
    {
        int index = curFolder->findFile(name);
        if (index == -1)
        {
            cout << "打开文件失败，没有找到文件" << endl;
            return false;
        }
    }

    // 关闭文件，指明文件名：1.撤销主存中的目录信息，并释放用户进程和文件之间的联系，2.考虑是否写回文件
    void closeFile(string name)
    {
    }
};

// 显示操作菜单
void showMenu()
{
    cout << "1.打开文件夹" << endl;
    cout << "2.创建文件夹" << endl;
    cout << "3.创建文件" << endl;
    cout << "4.删除文件" << endl;
    cout << "5.打开文件" << endl;
    cout << "6.关闭文件" << endl;
    cout << "7.调整操作权限" << endl;
    cout << "8.显示操作菜单" << endl;
    cout << "9.退出" << endl;
}

int main()
{
    FileSystem myFileSys(1024, 64, 8);

    cout << "文件系统已开始运行" << endl;
    cout << "请设置密码：";
    myFileSys.setPassword();

    showMenu();

    while (1)
    {
        string stmp;
        char ctmp;
        int itmp;

        cout << "请输入指令：";
        cin >> ctmp;

        cin.ignore();

        switch (ctmp)
        {
        case '1':
            cout << "( /x 为绝对路径, ./x为 当前目录, ../x 为上一级目录 )" << endl;
            cout << "请输入文件夹路径：";
            getline(cin, stmp);
            myFileSys.openFolder(stmp);
            myFileSys.showThisFolder(); // 显示当前文件夹内容
            break;
        case '2':
            cout << "请输入创建的文件夹名称：";
            getline(cin, stmp);
            myFileSys.createFolder(stmp);
            break;
        case '3':
            cout << "请输入创建的文件名称：";
            getline(cin, stmp);
            cout << "请输入保护等级：";
            cin >> itmp;
            myFileSys.createFile(stmp, 0, itmp);
            break;
        case '4':
            cout << "请输入删除的文件名称：";
            getline(cin, stmp);
            myFileSys.deleteFile(stmp);
            break;
        case '5':
            cout << "请输入打开的文件名称：";
            getline(cin, stmp);
            myFileSys.openFile(stmp);
            break;
        case '6':
            cout << "请输入关闭的文件名称：";
            getline(cin, stmp);
            myFileSys.closeFile(stmp);
            break;
        case '7':
            cout << "请输入密码：";
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
            cout << "输入错误，请重新输入" << endl;
            break;
        }
    }

    cout << "程序异常退出!!!" << endl;
    system("pause");
    return 0;
}
