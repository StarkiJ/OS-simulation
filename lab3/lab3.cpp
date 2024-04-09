#include "FileSystem.h"

// ��ʾ�����˵�
void showMenu()
{
    cout << "1.���ļ���" << endl;
    cout << "2.�����ļ���" << endl;
    cout << "3.�����ļ�" << endl;
    cout << "4.ɾ���ļ�" << endl;
    cout << "5.���ļ�" << endl;
    cout << "6.�ر��ļ�" << endl;
    cout << "7.���ļ�" << endl;
    cout << "8.д�ļ�" << endl;
    cout << "���� A ��������Ȩ��" << endl;
    cout << "���� M ��ʾ�����˵�" << endl;
    cout << "���� Q �˳�����" << endl;
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
            cout << "�����뱣���ȼ���";
            cin >> itmp;
            myFileSys.createFolder(stmp, itmp);
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
            cout << "������Ҫ��ȡ���ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.readFile(stmp);
            break;
        case '8':
            cout << "������Ҫд����ļ����ƣ�";
            getline(cin, stmp);
            cout << "������д������ݣ�";
            string data;
            cin >> data;
            myFileSys.writeFile(stmp, data);
            break;
        case 'A':
        case 'a':
            cout << "���������룺";
            getline(cin, stmp);
            myFileSys.adjustAuthority(stmp);
            break;
        case 'M':
        case 'm':
            showMenu();
            break;
        case 'Q':
        case 'q':
            cout << "�������˳�" << endl;
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
