#include "FileSystem.h"

// ��ʾ�����˵�
void showMenu()
{
    cout << "1.�����ļ���" << endl;
    cout << "2.���ļ���" << endl;
    cout << "3.�����ļ�" << endl;
    cout << "4.���ļ�" << endl;
    cout << "5.���ļ�" << endl;
    cout << "6.д�ļ�(��֧�����ģ�)" << endl;
    cout << "7.�ر��ļ�" << endl;
    cout << "8.ɾ���ļ�" << endl;
    cout << "���� M ��ʾ�����˵�" << endl;
    cout << "���� D ��ʾ��״Ŀ¼" << endl;
    cout << "���� O ��ʾ�򿪵��ļ�" << endl;
    cout << "���� A �������Ա����" << endl;
    cout << "���� Q �˳�����" << endl;
    cout << endl;
}

bool ifLegal(string name)
{
    int size = name.size();
    for (int i = 0; i < size; i++)
    {
        if (name[i] == '.' || name[i] == '/')
        {
            cout << "�������������в��ܰ�����.����/����" << endl;
            return false;
        }
    }
    return true;
}

int main()
{
    // ���һλ��������һ�����,�����ֵ�ռ��λ,�Ѷ�,���ǰѿ��С�ĳ��������
    // ���֧��������ɾ�Ĳ�Ҳ�Ѽ��ݣ����Ƿ������ļ���
    FileSystem myFileSys(1024, 64, 8);
    myFileSys.start();

    cout << "�ļ�ϵͳ�ѿ�ʼ����" << endl;

    showMenu();

    while (1)
    {
        string stmp;
        char ctmp;
        int itmp;

        cout << "������ָ�";
        cin >> stmp;
        cin.ignore();
        ctmp = stmp[0];

        switch (ctmp)
        {
        case '1':
            cout << "�����봴�����ļ������ƣ�";
            getline(cin, stmp);
            if (!ifLegal(stmp))
            {
                break;
            }
            myFileSys.createFolder(stmp, 0);
            break;
        case '2':
            cout << "( /x Ϊ����·��, ./xΪ ��ǰĿ¼, ../x Ϊ��һ��Ŀ¼ )" << endl;
            cout << "�������ļ���·����";
            getline(cin, stmp);
            myFileSys.openFolder(stmp);
            myFileSys.showPath();       // ��ʾ��ǰ·��
            myFileSys.showThisFolder(); // ��ʾ��ǰ�ļ�������
            break;
        case '3':
            cout << "�����봴�����ļ����ƣ�";
            getline(cin, stmp);
            if (!ifLegal(stmp))
            {
                break;
            }
            cout << "0.�����ļ�" << endl;
            cout << "1.��д�ļ�" << endl;
            cout << "2.ֻ���ļ�" << endl;
            cout << "�����뱣���ȼ���";
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
            cout << "������򿪵��ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.openFile(stmp);
            break;
        case '5':
            cout << "������Ҫ��ȡ���ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.readFile(stmp);
            break;
        case '6':
            cout << "������Ҫд����ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.writeFile(stmp);
            break;
        case '7':
            cout << "������رյ��ļ����ƣ�";
            getline(cin, stmp);
            myFileSys.closeFile(stmp);
            break;
        case '8':
            cout << "������ɾ�����ļ����ƣ�";
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
            cout << "���������룺";
            getline(cin, stmp);
            myFileSys.adjustAuthority(stmp);
            break;
        case 'Q':
        case 'q':
            myFileSys.end();
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
