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
        access = i;
        if (op.op == write)
        {
            dirty = true;
        }
    }
};

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
};

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

// 先进先出
void FIFO(vector<vector<int>> &externalMemory, vector<PageTableItem> &pageTable, vector<Operation> &operations)
{
    initPageTable(pageTable); // 初始化页表

    vector<vector<int>> physicalPage; // 物理页4x5

    int pageNum = 4;                       // 物理页数
    int pageSize = physicalPage[0].size(); // 页大小
    int ptlen = pageTable.size();          // 页表长度
    int oplen = operations.size();         // 操作数

    for (int i = 0; i < oplen; i++) // 遍历操作
    {
        Operation op = operations[i];
        int pageNum = op.logicalAddress / pageSize; // 页号
        int offset = op.logicalAddress % pageSize;  // 页内偏移
        if (pageNum < ptlen)
        {
            PageTableItem item = pageTable[pageNum];

            if (!item.valid) // 不在内存中
            {
                if (physicalPage.size() < pageNum) // 物理页没装满
                {
                    physicalPage.push_back(externalMemory[item.address]);
                    item.frameNumber = physicalPage.size() - 1;
                }
                else // 物理页已装满
                {
                    int replaced = 0;
                    for (int j = 0; j < ptlen; j++) // 找到最近最久未使用的页
                    {
                        if ((pageTable[j].access < pageTable[replaced].access) && (pageTable[j].valid))
                        {
                            replaced = j;
                        }
                    }
                    int framNum = pageTable[replaced].frameNumber; // 被替换的物理块号

                    pageTable[replaced].valid = false; // 无效化页表项
                    if (pageTable[replaced].dirty)     // 如果有过修改则需要写回
                    {
                        externalMemory[pageTable[replaced].address] = physicalPage[framNum];
                        pageTable[replaced].dirty = false;
                    }
                    physicalPage[framNum] = externalMemory[item.address]; // 更新物理块
                    item.frameNumber = framNum;                           // 更新页表对应的物理块号
                }
            }

            item.update(op, i); // 更新页表
        }
        else
        {
            cout << operations[i].logicalAddress << " 页号 > 页表长度，越界中断！!!" << endl;
            return;
        }
    }
}

int main()
{
    vector<Operation> operations; // 操作序列
    vector<vector<int>> externalMemory = {{0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4},
                                          {0, 1, 2, 3, 4}}; // 外存9x5
    vector<PageTableItem> pageTable;                        // 页表

    // 建立页表
    createPageTable(externalMemory, pageTable);
}