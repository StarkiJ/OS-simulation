#include "FileSystem.h"

// 显示操作菜单
void showMenu()
{
    cout << "1.打开文件夹" << endl;
    cout << "2.创建文件夹" << endl;
    cout << "3.创建文件" << endl;
    cout << "4.删除文件" << endl;
    cout << "5.打开文件" << endl;
    cout << "6.关闭文件" << endl;
    cout << "7.读文件" << endl;
    cout << "8.写文件" << endl;
    cout << "输入 A 调整操作权限" << endl;
    cout << "输入 M 显示操作菜单" << endl;
    cout << "输入 Q 退出程序" << endl;
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
            cout << "请输入保护等级：";
            cin >> itmp;
            myFileSys.createFolder(stmp, itmp);
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
            cout << "请输入要读取的文件名称：";
            getline(cin, stmp);
            myFileSys.readFile(stmp);
            break;
        case '8':
            cout << "请输入要写入的文件名称：";
            getline(cin, stmp);
            cout << "请输入写入的数据：";
            string data;
            cin >> data;
            myFileSys.writeFile(stmp, data);
            break;
        case 'A':
        case 'a':
            cout << "请输入密码：";
            getline(cin, stmp);
            myFileSys.adjustAuthority(stmp);
            break;
        case 'M':
        case 'm':
            showMenu();
            break;
        case 'Q':
        case 'q':
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
