#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int file()
{
    // ���ļ�
    std::ifstream file("xym1.txt");

    // ����ļ��Ƿ�ɹ���
    if (!file.is_open())
    {
        std::cerr << "�޷����ļ���" << std::endl;
        return 1;
    }

    // ʹ��peek()��������ļ��Ƿ�Ϊ��
    if (file.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "�ļ�Ϊ��" << std::endl;
    }
    else
    {
        std::cout << "�ļ���Ϊ��" << std::endl;
    }

    // �ر��ļ�
    file.close();

    return 0;
}

int pointer()
{
    int a[] = {1, 2, 3, 4, 5, 6, 7};
    int *p = &a[1];
    p = p + 2;
    *p = 100;
    cout << "*p = " << *p << endl;
    cout << "p = " << p << endl;
    cout << "&p = " << &p << endl;
    cout << "a[1] = " << a[1] << endl;
    cout << "&a[1] = " << &a[1] << endl;

    return 0;
}

int main()
{
    vector<queue<int>> vint;
    for (int i = 0; i < 10; i++)
    {
        queue<int> q;
        /*for (int j = 0; j < 10; j++)
        {
            q.push(j);
        }*/
        vint.push_back(q);
    }

    vint[0].push(100);

    int m = vint.size();
    for (int i = 0; i < m; i++)
    {
        int n = vint[i].size();
        for (int j = 0; j < n; j++)
        {
            cout << vint[i].front() << " ";
            vint[i].pop();
        }
        cout << endl;
    }

    return 0;
}