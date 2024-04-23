#include "FileSystem.h"

// 显示操作菜单
void showMenu()
{
    cout << "1.创建文件夹" << endl;
    cout << "2.打开文件夹" << endl;
    cout << "3.创建文件" << endl;
    cout << "4.打开文件" << endl;
    cout << "5.读文件" << endl;
    cout << "6.写文件(不支持中文！)" << endl;
    cout << "7.关闭文件" << endl;
    cout << "8.删除文件" << endl;
    cout << "输入 M 显示操作菜单" << endl;
    cout << "输入 D 显示树状目录" << endl;
    cout << "输入 O 显示打开的文件" << endl;
    cout << "输入 A 进入管理员界面" << endl;
    cout << "输入 Q 退出程序" << endl;
    cout << endl;
}

bool ifLegal(string name)
{
    int size = name.size();
    for (int i = 0; i < size; i++)
    {
        if (name[i] == '.' || name[i] == '/')
        {
            cout << "命名错误：名字中不能包含‘.’或‘/’！" << endl;
            return false;
        }
    }
    return true;
}

int main()
{
    // 最后一位用来存下一个块号,中文又得占两位,难顶,考虑把块大小改成奇数解决
    // 如果支持中文增删改插也难兼容，还是放弃中文兼容
    FileSystem myFileSys(1024, 64, 8);
    myFileSys.start();

    cout << "文件系统已开始运行" << endl;

    showMenu();

    while (1)
    {
        string stmp;
        char ctmp;
        int itmp;

        cout << "请输入指令：";
        cin >> stmp;
        cin.ignore();
        ctmp = stmp[0];

        switch (ctmp)
        {
        case '1':
            cout << "请输入创建的文件夹名称：";
            getline(cin, stmp);
            if (!ifLegal(stmp))
            {
                break;
            }
            myFileSys.createFolder(stmp, 0);
            break;
        case '2':
            cout << "( /x 为绝对路径, ./x为 当前目录, ../x 为上一级目录 )" << endl;
            cout << "请输入文件夹路径：";
            getline(cin, stmp);
            myFileSys.openFolder(stmp);
            myFileSys.showPath();       // 显示当前路径
            myFileSys.showThisFolder(); // 显示当前文件夹内容
            break;
        case '3':
            cout << "请输入创建的文件名称：";
            getline(cin, stmp);
            if (!ifLegal(stmp))
            {
                break;
            }
            cout << "0.公开文件" << endl;
            cout << "1.读写文件" << endl;
            cout << "2.只读文件" << endl;
            cout << "请输入保护等级：";
            cin >> itmp;
            if (itmp < 0)
            {
                itmp = 0;
            }
            if (itmp > 2)
            {
                itmp = 2;
            }
            myFileSys.createFile(stmp, 0, itmp);
            break;
        case '4':
            cout << "请输入打开的文件名称：";
            getline(cin, stmp);
            myFileSys.openFile(stmp);
            break;
        case '5':
            cout << "请输入要读取的文件名称：";
            getline(cin, stmp);
            myFileSys.readFile(stmp);
            break;
        case '6':
            cout << "请输入要写入的文件名称：";
            getline(cin, stmp);
            myFileSys.writeFile(stmp);
            break;
        case '7':
            cout << "请输入关闭的文件名称：";
            getline(cin, stmp);
            myFileSys.closeFile(stmp);
            break;
        case '8':
            cout << "请输入删除的文件名称：";
            getline(cin, stmp);
            myFileSys.deleteFile(stmp);
            break;
        case 'M':
        case 'm':
            showMenu();
            break;
        case 'D':
        case 'd':
            myFileSys.showTree();
            break;
        case 'O':
        case 'o':
            myFileSys.showOpenFile();
            break;
        case 'A':
        case 'a':
            cout << "请输入密码：";
            getline(cin, stmp);
            myFileSys.adjustAuthority(stmp);
            break;
        case 'Q':
        case 'q':
            myFileSys.end();
            cout << "程序已退出" << endl;
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
