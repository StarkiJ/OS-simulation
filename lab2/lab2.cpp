#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

#define read 0b0
#define write 0b1

// ����
class Operation
{
public:
    int logicalAddress; // �߼���ַ
    int op;             // ����
    int data;           // ����

    Operation()
    {
        logicalAddress = -1;
        op = -1;
    }

    void print()
    {
        cout << left << "|" << setw(10) << logicalAddress
             << "|" << setw(10) << (op == read ? "read" : "write")
             << "|" << setw(10) << (op == write ? data : -1) << endl;
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

    void update(Operation op) // ����ҳ����
    {
        valid = true;
        if (op.op == write)
        {
            dirty = true;
        }
    }
};

// ��ȡ�ⲿ�洢
bool readEM(vector<vector<int>> &externalMemory, int pageSize)
{
    ifstream ifs;
    ifs.open("externalMemory.txt", ios::in);

    // �ļ��������򷵻�
    if (!ifs)
    {
        cout << "�ļ�������" << endl;
        return 0;
    }

    // ���ļ��򷵻�
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "û���ⲿ�洢��Ϣ" << endl;
        return 0;
    }

    cout << "��ȡ�ⲿ�洢��Ϣ" << endl;

    while (!ifs.eof())
    {
        vector<int> temp;
        for (int i = 0; i < pageSize; i++)
        {
            int data;
            ifs >> data;
            cout << left << setw(4) << data;
            temp.push_back(data);
        }
        cout << endl;
        externalMemory.push_back(temp);
    }

    ifs.close();
    return 1;
}

// ��ȡ�洢�Ĳ�������
bool readFile(vector<Operation> &operations, int pageSize)
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
         << "|" << setw(10) << "��������"
         << "|" << setw(10) << "����" << endl;

    while (!ifs.eof())
    {
        Operation op;
        ifs >> op.logicalAddress >> op.op;
        if (op.op == 1)
        {
            int data;
            ifs >> op.data;
        }
        op.print();
        operations.push_back(op);
    }

    ifs.close();
    return 1;
}

// ��ʾ�洢��Ϣ
void showMemory(vector<vector<int>> &memory)
{
    int len = memory.size();
    cout << "��ʾ��棺" << endl;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < memory[i].size(); j++)
        {
            cout << left << setw(4) << memory[i][j];
        }
        cout << endl;
    }
    cout << endl;
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

// ��ʾ����ҳ
void showPhyicalPage(vector<PageTableItem> &PageTable, int phyNum, bool ifclock)
{
    int len = PageTable.size();

    for (int j = 0; j < phyNum; j++)
    {
        for (int i = 0; i < len; i++)
        {
            PageTableItem item = PageTable[i];
            if (item.valid && item.frameNumber == j)
            {
                cout << item.frameNumber << " " << i;
                if (ifclock && item.access)
                {
                    cout << "*";
                }
                cout << endl;
                break;
            }
        }
    }
}

// �Ƚ��ȳ�FIFO
void firstInFirstOut(vector<vector<int>> &externalMemory, vector<Operation> &operations, int phyNum)
{
    cout << "�Ƚ��ȳ���FIFO���㷨��ʼ" << endl;

    vector<vector<int>> physicalPage;           // ����ҳ
    vector<PageTableItem> pageTable;            // ҳ��
    createPageTable(externalMemory, pageTable); // ����ҳ��

    int pageSize = externalMemory[0].size(); // ҳ��С
    int ptlen = pageTable.size();            // ҳ����
    int oplen = operations.size();           // ������
    int miss = 0;                            // ȱҳ����

    for (int i = 0; i < oplen; i++) // ��������
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // ҳ��
        int offset = op.logicalAddress % pageSize;  // ҳ��ƫ��

        cout << "������";
        op.print();
        cout << "ҳ�ţ�" << pageNum << " ƫ�ƣ�" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " ҳ�� > ҳ���ȣ�Խ���жϣ�!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // ��Ӧ��ҳ����

        if (!item.valid) // �����ڴ���
        {
            cout << "ȱҳ�������ڴ���" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // ����ҳûװ��
            {
                item.frameNumber = physicalPage.size();
                physicalPage.push_back(externalMemory[item.address]);
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

            item.access = i; // ����ҳ�����ʱ��
        }
        else
        {
            cout << "���У����ڴ���" << endl;
        }

        if (op.op == write)
        {
            physicalPage[item.frameNumber][offset] = op.data;
        }

        item.update(op); // ����ҳ��
        pageTable[pageNum] = item;

        cout << "�����ַ��" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 0);
    }

    cout << "�Ƚ��ȳ���FIFO���㷨����" << endl;
    cout << "ȱҳ�ʣ�" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

// �������ʹ��LRU
void leastRecentlyUsed(vector<vector<int>> &externalMemory, vector<Operation> &operations, int phyNum)
{
    cout << "�������ʹ�ã�LRU���㷨��ʼ" << endl;

    vector<vector<int>> physicalPage;           // ����ҳ
    vector<PageTableItem> pageTable;            // ҳ��
    createPageTable(externalMemory, pageTable); // ����ҳ��

    int pageSize = externalMemory[0].size(); // ҳ��С
    int ptlen = pageTable.size();            // ҳ����
    int oplen = operations.size();           // ������
    int miss = 0;                            // ȱҳ����

    for (int i = 0; i < oplen; i++) // ��������
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // ҳ��
        int offset = op.logicalAddress % pageSize;  // ҳ��ƫ��

        cout << "������" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "ҳ�ţ�" << pageNum << " ƫ�ƣ�" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " ҳ�� > ҳ���ȣ�Խ���жϣ�!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // ��Ӧ��ҳ����

        if (!item.valid) // �����ڴ���
        {
            cout << "ȱҳ�������ڴ���" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // ����ҳûװ��
            {
                item.frameNumber = physicalPage.size();
                physicalPage.push_back(externalMemory[item.address]);
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

        if (op.op == write)
        {
            physicalPage[item.frameNumber][offset] = op.data;
        }

        item.access = i;
        item.update(op); // ����ҳ��
        pageTable[pageNum] = item;

        cout << "�����ַ��" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 0);
    }

    cout << "�������ʹ�ã�LRU���㷨����" << endl;
    cout << "ȱҳ�ʣ�" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

// ʱ���û�CLOCK �� ���δʹ�ã�Not Recently Used��NRU��
void clock(vector<vector<int>> &externalMemory, vector<Operation> &operations, int phyNum)
{
    cout << "ʱ�ӣ�CLOCK���㷨��ʼ" << endl;

    vector<vector<int>> physicalPage;           // ����ҳ
    vector<PageTableItem> pageTable;            // ҳ��
    createPageTable(externalMemory, pageTable); // ����ҳ��

    int pageSize = externalMemory[0].size(); // ҳ��С
    int ptlen = pageTable.size();            // ҳ����
    int oplen = operations.size();           // ������
    int miss = 0;                            // ȱҳ����
    int ptr = 0;                             // �滻ָ��

    for (int i = 0; i < oplen; i++) // ��������
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // ҳ��
        int offset = op.logicalAddress % pageSize;  // ҳ��ƫ��

        cout << "������" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "ҳ�ţ�" << pageNum << " ƫ�ƣ�" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " ҳ�� > ҳ���ȣ�Խ���жϣ�!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // ��Ӧ��ҳ����

        if (!item.valid) // �����ڴ���
        {
            cout << "ȱҳ�������ڴ���" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // ����ҳûװ��
            {
                item.frameNumber = physicalPage.size();
                physicalPage.push_back(externalMemory[item.address]);
                ptr = (ptr + 1) % phyNum; // �滻ָ�����
            }
            else // ����ҳ��װ��
            {
                int replaced;
                for (int j = 0; j < ptlen;) // �ҵ�������δʹ�õ�ҳ
                {
                    if ((pageTable[j].valid) && (pageTable[j].frameNumber == ptr)) // �ҵ�����ʹ������ҳ��ҳ����
                    {
                        if (pageTable[j].access == 1)
                        {
                            pageTable[j].access = 0;
                            ptr = (ptr + 1) % phyNum; // �滻ָ�����
                        }
                        else
                        {
                            replaced = j;
                            break;
                        }
                    }
                    j = (j + 1) % ptlen;
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

                ptr = (ptr + 1) % phyNum; // �滻ָ�����
            }
        }
        else
        {
            cout << "���У����ڴ���" << endl;
        }

        if (op.op == write)
        {
            physicalPage[item.frameNumber][offset] = op.data;
        }

        item.access = 1;
        item.update(op); // ����ҳ��
        pageTable[pageNum] = item;

        cout << "�����ַ��" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 1);
        cout << "time ptr: " << ptr << endl;
    }

    cout << "ʱ�ӣ�CLOCK���㷨����" << endl;
    cout << "ȱҳ�ʣ�" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

// ����û�
void randomReplace(vector<vector<int>> &externalMemory, vector<Operation> &operations, int phyNum)
{
    cout << "����û��㷨��ʼ" << endl;

    srand((unsigned)time(NULL)); // ���������

    vector<vector<int>> physicalPage;           // ����ҳ
    vector<PageTableItem> pageTable;            // ҳ��
    createPageTable(externalMemory, pageTable); // ����ҳ��

    int pageSize = externalMemory[0].size(); // ҳ��С
    int ptlen = pageTable.size();            // ҳ����
    int oplen = operations.size();           // ������
    int miss = 0;                            // ȱҳ����

    for (int i = 0; i < oplen; i++) // ��������
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // ҳ��
        int offset = op.logicalAddress % pageSize;  // ҳ��ƫ��

        cout << "������" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "ҳ�ţ�" << pageNum << " ƫ�ƣ�" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " ҳ�� > ҳ���ȣ�Խ���жϣ�!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // ��Ӧ��ҳ����

        if (!item.valid) // �����ڴ���
        {
            cout << "ȱҳ�������ڴ���" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // ����ҳûװ��
            {
                item.frameNumber = physicalPage.size();
                physicalPage.push_back(externalMemory[item.address]);
            }
            else // ����ҳ��װ��
            {
                int framNum = rand() % phyNum; // ���ѡ���滻��������
                int replaced;                  // ���滻��������Ӧ��ҳ��

                for (int j = 0; j < ptlen; j++)
                {
                    if (pageTable[j].valid && (pageTable[j].frameNumber == framNum)) // �ҵ�����ʹ��Ŀ������ҳ��ҳ����
                    {
                        replaced = j;
                        break;
                    }
                }

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

        if (op.op == write)
        {
            physicalPage[item.frameNumber][offset] = op.data;
        }

        item.update(op); // ����ҳ��
        pageTable[pageNum] = item;

        cout << "�����ַ��" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 0);
    }

    cout << "����û��㷨����" << endl;
    cout << "ȱҳ�ʣ�" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

int main()
{
    vector<Operation> operations;       // ��������
    vector<vector<int>> externalMemory; // ���

    int pageSize = 8;
    int phyNum = 3;

    if (readEM(externalMemory, pageSize)) // ��ȡ���
    {
        cout << "��ȡ���ɹ�" << endl;

        if (readFile(operations, pageSize))
        {

            cout << "��ȡ�����ɹ�" << endl;

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
                    firstInFirstOut(externalMemory, operations, phyNum);
                    break;
                case 2:
                    leastRecentlyUsed(externalMemory, operations, phyNum);
                    break;
                case 3:
                    clock(externalMemory, operations, phyNum);
                    break;
                case 4:
                    randomReplace(externalMemory, operations, phyNum);
                    break;
                default:
                    cout << "�������" << endl;
                }

                showMemory(externalMemory);

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
    }
    else
    {
        cout << "��ȡ���ʧ��" << endl;
    }

    system("pause");
    return 0;
}