#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

#define read 0b0
#define write 0b1

// ����
class Operation
{
public:
    int logicalAddress; // �߼���ַ
    int op;             // ����

    Operation()
    {
        logicalAddress = -1;
        op = -1;
    }

    void print()
    {
        cout << left << "|" << setw(10) << logicalAddress
             << "|" << setw(10) << (op == read ? "read" : "write") << endl;
    }
};

// ҳ����
class PageTableItem
{
public:
    int address;     // ����ַ
    int frameNumber; // ������
    int access;      // �����ֶ�
    bool valid;      // ��Чλ
    bool dirty;      // ��λ

    PageTableItem()
    {
        address = -1;
        frameNumber = -1;
        access = -1;
        valid = false;
        dirty = false;
    }

    void init()
    {
        frameNumber = -1;
        access = -1;
        valid = false;
        dirty = false;
    }

    void update(Operation op, int i) // ����ҳ����
    {
        valid = true;
        access = i;
        if (op.op == write)
        {
            dirty = true;
        }
    }
};

// ��ȡ�洢�ļƻ�
bool readFile(vector<Operation> &operations)
{
    ifstream ifs;
    ifs.open("lab2.txt", ios::in);

    // �ļ��������򷵻�
    if (!ifs)
    {
        cout << "�ļ�������" << endl;
        return 0;
    }

    // ���ļ��򷵻�
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "û�в�����Ϣ" << endl;
        return 0;
    }

    cout << "��ȡ������Ϣ" << endl;
    cout << left << "|" << setw(10) << "�߼���ַ"
         << "|" << setw(10) << "��������" << endl;

    // ��ȡ������Ϣ
    while (!ifs.eof())
    {
        Operation op;
        ifs >> op.logicalAddress >> op.op;
        op.print();
        operations.push_back(op);
    }

    ifs.close();
    return 1;
}

// ����ҳ��
void createPageTable(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable)
{
    int len = externalMemory.size();
    for (int i = 0; i < len; i++)
    {
        PageTableItem item;
        item.address = i; // ����ַ
        pageTable.push_back(item);
    }
}

// ��ʼ��ҳ��
void initPageTable(vector<PageTableItem> &pageTable)
{
    int len = pageTable.size();
    for (int i = 0; i < len; i++)
    {
        pageTable[i].init();
    }
}

// ��ʾ����ҳ
void showPhyicalPage(vector<PageTableItem> &PageTable, int phyNum)
{
    int len = PageTable.size();

    for (int j = 0; j < phyNum; j++)
    {
        for (int i = 0; i < len; i++)
        {
            if (PageTable[i].valid && PageTable[i].frameNumber == j)
            {
                cout << PageTable[i].frameNumber << " " << i << endl;
                break;
            }
        }
    }
}

// �Ƚ��ȳ�FIFO
void firstInFirstOut(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable, vector<Operation> &operations, int phyNum)
{
    cout << "�Ƚ��ȳ��㷨��ʼ" << endl;

    initPageTable(pageTable); // ��ʼ��ҳ��

    vector<vector<int>> physicalPage; // ����ҳ4x8

    int pageSize = externalMemory[0].size(); // ҳ��С
    int ptlen = pageTable.size();            // ҳ����
    int oplen = operations.size();           // ������

    for (int i = 0; i < oplen; i++) // ��������
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // ҳ��
        int offset = op.logicalAddress % pageSize;  // ҳ��ƫ��

        cout << "������" << op.logicalAddress << " " << op.op << endl;
        cout << "ҳ�ţ�" << pageNum << " ƫ�ƣ�" << offset << endl;

        if (pageNum < ptlen)
        {
            PageTableItem item = pageTable[pageNum]; // ��Ӧ��ҳ����

            if (!item.valid) // �����ڴ���
            {
                cout << "ȱҳ�������ڴ���" << endl;

                if (physicalPage.size() < phyNum) // ����ҳûװ��
                {
                    physicalPage.push_back(externalMemory[item.address]);
                    item.frameNumber = physicalPage.size() - 1;
                }
                else // ����ҳ��װ��
                {
                    int replaced = -1;              // Ĭ��Ϊ��
                    for (int j = 0; j < ptlen; j++) // �ҵ�������δʹ�õ�ҳ
                    {
                        if (pageTable[j].valid) // �ҵ�����ʹ������ҳ��ҳ����
                        {
                            if (replaced == -1) // ��һ���ҵ�
                            {
                                replaced = j;
                            }
                            else if (pageTable[j].access < pageTable[replaced].access) // �ҵ�����δʹ�õ�ҳ
                            {
                                replaced = j;
                            }
                        }
                    }
                    int framNum = pageTable[replaced].frameNumber; // ���滻��������

                    pageTable[replaced].valid = false; // ��Ч��ҳ����
                    if (pageTable[replaced].dirty)     // ����й��޸�����Ҫд��
                    {
                        externalMemory[pageTable[replaced].address] = physicalPage[framNum];
                        pageTable[replaced].dirty = false;

                        cout << "д�أ�" << framNum << " " << pageTable[replaced].address << endl;
                    }

                    physicalPage[framNum] = externalMemory[item.address]; // ���������
                    item.frameNumber = framNum;                           // ����ҳ���Ӧ��������
                }
            }
            else
            {
                cout << "���У����ڴ���" << endl;
            }

            item.update(op, i); // ����ҳ��
            pageTable[pageNum] = item;

            showPhyicalPage(pageTable, phyNum);
        }
        else
        {
            cout << operations[i].logicalAddress << " ҳ�� > ҳ���ȣ�Խ���жϣ�!!" << endl;
            return;
        }
    }
}

int main()
{
    vector<Operation> operations; // ��������
    vector<vector<int>> externalMemory = {{0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7},
                                          {0, 1, 2, 3, 4, 5, 6, 7}}; // ���10x8
    vector<PageTableItem> pageTable;                                 // ҳ��

    // ����ҳ��
    createPageTable(externalMemory, pageTable);

    if (readFile(operations))
    {

        cout << "��ȡ���̳ɹ�" << endl;

        while (1)
        {
            // ѡ������㷨
            cout << "��ѡ���û��㷨��" << endl;
            cout << "1.�Ƚ��ȳ���FIFO��" << endl;
            cout << "2.�������ʹ�ã�LRU��" << endl;
            cout << "3.ʱ���û���CLOCK��" << endl;
            cout << "4.����û�" << endl;
            cout << "����������ѡ��" << endl;
            int choice = 0;
            cin >> choice;
            switch (choice)
            {
            case 1:
                firstInFirstOut(externalMemory, pageTable, operations, 4);
                break;
            case 2:
                // LRU();
                break;
            case 3:
                // CLOCK();
                break;
            case 4:
                // randomReplace();
                break;
            default:
                cout << "�������" << endl;
            }

            cout << "�Ƿ�����ѡ���㷨��(y: ����ѡ��, n: ��������)" << endl;
            char c;
            cin >> c;
            if (c != 'Y' && c != 'y')
            {
                break;
            }
        }
    }
    else
    {
        cout << "��ȡ����ʧ��" << endl;
    }

    system("pause");
    return 0;
}