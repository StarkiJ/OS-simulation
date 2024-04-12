#include "FileSystem.h"

// ��ʾ�����˵�
void showMenu()
{
    cout << "1.�����ļ���" << endl;
    cout << "2.���ļ���" << endl;
    cout << "3.�����ļ�" << endl;
    cout << "4.���ļ�" << endl;
    cout << "5.���ļ�" << endl;
    cout << "6.д�ļ�" << endl;
    cout << "7.�ر��ļ�" << endl;
    cout << "8.ɾ���ļ�" << endl;
    cout << "���� A �������Ա����" << endl;
    cout << "���� M ��ʾ�����˵�" << endl;
    cout << "���� Q �˳�����" << endl;
}

int main()
{
    // ���һλ��������һ�����,�����ֵ�ռ��λ,�Ѷ�,���ǰѿ��С�ĳ��������
    // ���֧��������ɾ�Ĳ�Ҳ�Ѽ��ݣ����Ƿ������ļ���
    FileSystem myFileSys(1024, 64, 8);

    cout << "�ļ�ϵͳ�ѿ�ʼ����" << endl;
    // cout << "���������룺";
    // myFileSys.setPassword();

    showMenu();

    while (1)
    {
        string stmp;
        char ctmp;
        int itmp;

        cout << "������ָ�";
        getline(cin, stmp);
        ctmp = stmp[0];

        switch (ctmp)
        {
        case '1':
            cout << "�����봴�����ļ������ƣ�";
            getline(cin, stmp);
            myFileSys.createFolder(stmp, 0);
            break;
        case '2':
            cout << "( /x Ϊ����·��, ./xΪ ��ǰĿ¼, ../x Ϊ��һ��Ŀ¼ )" << endl;
            cout << "�������ļ���·����";
            getline(cin, stmp);
            myFileSys.openFolder(stmp);
            myFileSys.showThisFolder(); // ��ʾ��ǰ�ļ�������
            break;
        case '3':
            cout << "�����봴�����ļ����ƣ�";
            getline(cin, stmp);
            cout << "0.�����ļ�" << endl;
            cout << "1.��д�ļ�" << endl;
            cout << "2.ֻ���ļ�" << endl;
            // cout << "�����뱣���ȼ���";
            // cin >> itmp;
            itmp = 0;
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
