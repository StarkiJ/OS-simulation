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

// 操作
class Operation
{
public:
    int logicalAddress; // 逻辑地址
    int op;             // 操作

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

// 页表项
class PageTableItem
{
public:
    int address;     // 外存地址
    int frameNumber; // 物理块号
    int access;      // 访问字段
    bool valid;      // 有效位
    bool dirty;      // 脏位

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

    void update(Operation op, int i) // 更新页表项
    {
        valid = true;
        // access = i;
        if (op.op == write)
        {
            dirty = true;
        }
    }
};

// 读取外部存储
bool readEM(vector<vector<int>> &externalMemory, int pageSize)
{
    ifstream ifs;
    ifs.open("externalMemory.txt", ios::in);

    // 文件不存在则返回
    if (!ifs)
    {
        cout << "文件不存在" << endl;
        return 0;
    }

    // 空文件则返回
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "没有外部存储信息" << endl;
        return 0;
    }

    cout << "读取外部存储信息" << endl;

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

// 读取存储的操作序列
bool readFile(vector<Operation> &operations)
{
    ifstream ifs;
    ifs.open("lab2.txt", ios::in);

    // 文件不存在则返回
    if (!ifs)
    {
        cout << "文件不存在" << endl;
        return 0;
    }

    // 空文件则返回
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "没有操作信息" << endl;
        return 0;
    }

    cout << "读取操作信息" << endl;
    cout << left << "|" << setw(10) << "逻辑地址"
         << "|" << setw(10) << "操作类型" << endl;

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

// 建立页表
void createPageTable(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable)
{
    int len = externalMemory.size();
    for (int i = 0; i < len; i++)
    {
        PageTableItem item;
        item.address = i; // 外存地址
        pageTable.push_back(item);
    }
}

// 初始化页表
void initPageTable(vector<PageTableItem> &pageTable)
{
    int len = pageTable.size();
    for (int i = 0; i < len; i++)
    {
        pageTable[i].init();
    }
}

// 显示物理页
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

// 先进先出FIFO
void firstInFirstOut(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable, vector<Operation> &operations, int phyNum)
{
    cout << "先进先出（FIFO）算法开始" << endl;

    initPageTable(pageTable); // 初始化页表

    vector<vector<int>> physicalPage; // 物理页

    int pageSize = externalMemory[0].size(); // 页大小
    int ptlen = pageTable.size();            // 页表长度
    int oplen = operations.size();           // 操作数
    int miss = 0;                            // 缺页次数

    for (int i = 0; i < oplen; i++) // 遍历操作
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // 页号
        int offset = op.logicalAddress % pageSize;  // 页内偏移

        cout << "操作：" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "页号：" << pageNum << " 偏移：" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " 页号 > 页表长度，越界中断！!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // 对应的页表项

        if (!item.valid) // 不在内存中
        {
            cout << "缺页：不在内存中" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // 物理页没装满
            {
                physicalPage.push_back(externalMemory[item.address]);
                item.frameNumber = physicalPage.size() - 1;
            }
            else // 物理页已装满
            {
                int replaced = -1;              // 默认为负
                for (int j = 0; j < ptlen; j++) // 找到最近最久未使用的页
                {
                    if (pageTable[j].valid) // 找到正在使用物理页的页表项
                    {
                        if (replaced == -1) // 第一次找到
                        {
                            replaced = j;
                        }
                        else if (pageTable[j].access < pageTable[replaced].access) // 找到更久未使用的页
                        {
                            replaced = j;
                        }
                    }
                }
                int framNum = pageTable[replaced].frameNumber; // 被替换的物理块号

                pageTable[replaced].valid = false; // 无效化页表项
                if (pageTable[replaced].dirty)     // 如果有过修改则需要写回
                {
                    externalMemory[pageTable[replaced].address] = physicalPage[framNum];
                    pageTable[replaced].dirty = false;

                    cout << "写回：" << framNum << " " << pageTable[replaced].address << endl;
                }

                physicalPage[framNum] = externalMemory[item.address]; // 更新物理块
                item.frameNumber = framNum;                           // 更新页表对应的物理块号
            }

            item.access = i; // 更新页表访问时间
        }
        else
        {
            cout << "命中：在内存中" << endl;
        }

        item.update(op, i); // 更新页表
        pageTable[pageNum] = item;

        cout << "物理地址：" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 0);
    }

    cout << "先进先出（FIFO）算法结束" << endl;
    cout << "缺页率：" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

// 最近最少使用LRU
void leastRecentlyUsed(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable, vector<Operation> &operations, int phyNum)
{
    cout << "最近最少使用（LRU）算法开始" << endl;

    initPageTable(pageTable); // 初始化页表

    vector<vector<int>> physicalPage; // 物理页4x8

    int pageSize = externalMemory[0].size(); // 页大小
    int ptlen = pageTable.size();            // 页表长度
    int oplen = operations.size();           // 操作数
    int miss = 0;                            // 缺页次数

    for (int i = 0; i < oplen; i++) // 遍历操作
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // 页号
        int offset = op.logicalAddress % pageSize;  // 页内偏移

        cout << "操作：" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "页号：" << pageNum << " 偏移：" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " 页号 > 页表长度，越界中断！!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // 对应的页表项

        if (!item.valid) // 不在内存中
        {
            cout << "缺页：不在内存中" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // 物理页没装满
            {
                physicalPage.push_back(externalMemory[item.address]);
                item.frameNumber = physicalPage.size() - 1;
            }
            else // 物理页已装满
            {
                int replaced = -1;              // 默认为负
                for (int j = 0; j < ptlen; j++) // 找到最近最久未使用的页
                {
                    if (pageTable[j].valid) // 找到正在使用物理页的页表项
                    {
                        if (replaced == -1) // 第一次找到
                        {
                            replaced = j;
                        }
                        else if (pageTable[j].access < pageTable[replaced].access) // 找到更久未使用的页
                        {
                            replaced = j;
                        }
                    }
                }
                int framNum = pageTable[replaced].frameNumber; // 被替换的物理块号

                pageTable[replaced].valid = false; // 无效化页表项
                if (pageTable[replaced].dirty)     // 如果有过修改则需要写回
                {
                    externalMemory[pageTable[replaced].address] = physicalPage[framNum];
                    pageTable[replaced].dirty = false;

                    cout << "写回：" << framNum << " " << pageTable[replaced].address << endl;
                }

                physicalPage[framNum] = externalMemory[item.address]; // 更新物理块
                item.frameNumber = framNum;                           // 更新页表对应的物理块号
            }
        }
        else
        {
            cout << "命中：在内存中" << endl;
        }

        item.access = i;
        item.update(op, i); // 更新页表
        pageTable[pageNum] = item;

        cout << "物理地址：" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 0);
    }

    cout << "最近最少使用（LRU）算法结束" << endl;
    cout << "缺页率：" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

// 时钟置换CLOCK 即 最近未使用（Not Recently Used，NRU）
void clock(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable, vector<Operation> &operations, int phyNum)
{
    cout << "时钟（CLOCK）算法开始" << endl;

    initPageTable(pageTable); // 初始化页表

    vector<vector<int>> physicalPage; // 物理页4x8

    int pageSize = externalMemory[0].size(); // 页大小
    int ptlen = pageTable.size();            // 页表长度
    int oplen = operations.size();           // 操作数
    int miss = 0;                            // 缺页次数
    int ptr = 0;                             // 替换指针

    for (int i = 0; i < oplen; i++) // 遍历操作
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // 页号
        int offset = op.logicalAddress % pageSize;  // 页内偏移

        cout << "操作：" << op.logicalAddress << " " << (op.op == read ? "read" : "write") << endl;
        cout << "页号：" << pageNum << " 偏移：" << offset << endl;

        if (pageNum >= ptlen)
        {
            cout << operations[i].logicalAddress << " 页号 > 页表长度，越界中断！!!" << endl;
            return;
        }

        PageTableItem item = pageTable[pageNum]; // 对应的页表项

        if (!item.valid) // 不在内存中
        {
            cout << "缺页：不在内存中" << endl;

            miss++;

            if (physicalPage.size() < phyNum) // 物理页没装满
            {
                physicalPage.push_back(externalMemory[item.address]);
                item.frameNumber = physicalPage.size() - 1;
                ptr = (ptr + 1) % phyNum; // 替换指针后移
            }
            else // 物理页已装满
            {
                int replaced;
                for (int j = 0; j < ptlen;) // 找到最近最久未使用的页
                {
                    if ((pageTable[j].valid) && (pageTable[j].frameNumber == ptr)) // 找到正在使用物理页的页表项
                    {
                        if (pageTable[j].access == 1)
                        {
                            pageTable[j].access = 0;
                            ptr = (ptr + 1) % phyNum; // 替换指针后移
                        }
                        else
                        {
                            replaced = j;
                            break;
                        }
                    }
                    j = (j + 1) % ptlen;
                }
                int framNum = pageTable[replaced].frameNumber; // 被替换的物理块号

                pageTable[replaced].valid = false; // 无效化页表项
                if (pageTable[replaced].dirty)     // 如果有过修改则需要写回
                {
                    externalMemory[pageTable[replaced].address] = physicalPage[framNum];
                    pageTable[replaced].dirty = false;

                    cout << "写回：" << framNum << " " << pageTable[replaced].address << endl;
                }

                physicalPage[framNum] = externalMemory[item.address]; // 更新物理块
                item.frameNumber = framNum;                           // 更新页表对应的物理块号

                ptr = (ptr + 1) % phyNum; // 替换指针后移
            }
        }
        else
        {
            cout << "命中：在内存中" << endl;
        }

        item.access = 1;
        item.update(op, i); // 更新页表
        pageTable[pageNum] = item;

        cout << "物理地址：" << item.frameNumber << offset << endl;

        showPhyicalPage(pageTable, phyNum, 1);
        cout << "time ptr: " << ptr << endl;
    }

    cout << "时钟（CLOCK）算法结束" << endl;
    cout << "缺页率：" << ((double)miss / (double)oplen) * 100 << " %" << endl;
}

int main()
{
    vector<Operation> operations;       // 操作序列
    vector<vector<int>> externalMemory; // 外存
    vector<PageTableItem> pageTable;    // 页表

    if (readEM(externalMemory, 8)) // 读取外存
    {
        cout << "读取外存成功" << endl;

        createPageTable(externalMemory, pageTable); // 建立页表

        if (readFile(operations))
        {

            cout << "读取操作成功" << endl;

            while (1)
            {
                // 选择调度算法
                cout << "请选择置换算法：" << endl;
                cout << "1.先进先出（FIFO）" << endl;
                cout << "2.最近最少使用（LRU）" << endl;
                cout << "3.时钟置换（CLOCK）" << endl;
                cout << "4.随机置换" << endl;
                cout << "请输入您的选择：" << endl;
                int choice = 0;
                cin >> choice;
                switch (choice)
                {
                case 1:
                    firstInFirstOut(externalMemory, pageTable, operations, 3);
                    break;
                case 2:
                    leastRecentlyUsed(externalMemory, pageTable, operations, 3);
                    break;
                case 3:
                    clock(externalMemory, pageTable, operations, 3);
                    break;
                case 4:
                    // randomReplace();
                    break;
                default:
                    cout << "输入错误" << endl;
                }

                cout << "是否重新选择算法？(y: 重新选择, n: 结束程序)" << endl;
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
            cout << "读取操作失败" << endl;
        }
    }
    else
    {
        cout << "读取外存失败" << endl;
    }

    system("pause");
    return 0;
}