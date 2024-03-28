#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

// 设置进程类
class Process
{
public:
    string id;                     // 进程id
    double arrivalTime;            // 到达时间
    double serviceTime;            // 服务时间
    double priority;               // 优先级
    double startTime;              // 开始时间
    double endTime;                // 结束时间
    double responseTime;           // 响应时间
    double turnaroundTime;         // 周转时间
    double weightedTurnaroundTime; // 带权周转时间
    double waitTime;               // 等待时间
    double remainTime;             // 剩余时间

    Process()
    {
        this->id = "-1";
        this->arrivalTime = -1;
        this->serviceTime = -1;
        this->priority = -1;
        this->startTime = -1;
        this->endTime = -1;
        this->responseTime = -1;
        this->turnaroundTime = -1;
        this->weightedTurnaroundTime = -1;
        this->waitTime = -1;
        this->remainTime = -1;
    }

    void print()
    {
        cout << left << "|" << setw(10) << id << "|" << setw(10) << arrivalTime << "|" << setw(10) << serviceTime << endl;
    }

    void printResult()
    {
        cout << left << "|" << setw(10) << id << "|" << setw(10) << arrivalTime << "|" << setw(10) << serviceTime << "|"
             << setw(10) << priority + 1 << "|" << setw(10) << startTime << "|" << setw(10) << endTime << "|"
             << setw(10) << responseTime << "|" << setw(10) << turnaroundTime << "|"
             << setw(14) << weightedTurnaroundTime << "|" << setw(10) << waitTime << endl;
    }

    // 进程完成后计算相关时间
    void calculateTime()
    {
        if (startTime < 0)
        {
            cout << "进程 " << id << " 未开始运行!!!" << endl;
            return;
        }
        if (endTime < 0)
        {
            cout << "进程 " << id << " 未结束运行!!!" << endl;
            return;
        }

        responseTime = startTime - arrivalTime;                // 响应时间=到达时间-开始时间
        turnaroundTime = endTime - arrivalTime;                // 周转时间=结束时间-到达时间
        weightedTurnaroundTime = turnaroundTime / serviceTime; // 带权周转时间=周转时间/服务时间
        waitTime = turnaroundTime - serviceTime;               // 等待时间=周转时间-服务时间
    }
};

// 比较服务时间大小
bool compareServiceTime(Process a, Process b)
{
    return a.serviceTime < b.serviceTime;
}

// 读取存储的计划
bool readFile(vector<Process> &processes)
{
    ifstream ifs;
    ifs.open("xym1.txt", ios::in);

    // 文件不存在则返回
    if (!ifs)
    {
        cout << "文件不存在" << endl;
        return 0;
    }

    // 空文件则返回
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "没有进程信息" << endl;
        return 0;
    }

    cout << "读取进程信息" << endl;
    cout << left << "|" << setw(10) << "进程ID"
         << "|" << setw(10) << "到达时间"
         << "|" << setw(10) << "服务时间" << endl;

    // 读取进程信息
    while (!ifs.eof())
    {
        Process s;
        ifs >> s.id >> s.arrivalTime >> s.serviceTime;
        s.remainTime = s.serviceTime;
        s.print();
        processes.push_back(s);
    }

    ifs.close();
    return 1;
}

// 显示结果
void showResult(vector<Process> &processes)
{
    int n = processes.size();

    cout << "显示结果" << endl;
    cout << left << "|" << setw(10) << "进程ID"
         << "|" << setw(10) << "到达时间"
         << "|" << setw(10) << "服务时间"
         << "|" << setw(10) << "优先级"
         << "|" << setw(10) << "开始时间"
         << "|" << setw(10) << "结束时间"
         << "|" << setw(10) << "响应时间"
         << "|" << setw(10) << "周转时间"
         << "|" << setw(14) << "带权周转时间"
         << "|" << setw(10) << "等待时间" << endl;

    for (int i = 0; i < n; i++)
    {
        Process p = processes[i];
        p.printResult();
    }
}

// 计算平均周转时间/平均带权周转时间，平均等待时间、平均响应时间、利用率等性能指标及公平性指标
void performanceIndicators(vector<Process> &processes)
{
    cout << "开始计算性能指标" << endl;

    showResult(processes);

    Process p;
    int n = processes.size();               // 进程数量
    double totalServiceTime = 0;            //
    double totalResponseTime = 0;           // 总响应时间
    double totalTurnaroundTime = 0;         // 总周转时间
    double totalWeightedTurnaroundTime = 0; // 总带权周转时间
    double totalWaitTime = 0;               // 总等待时间
    double Utilization = 0;                 // 利用率

    for (int i = 0; i < n; i++)
    {
        p = processes[i];
        if (p.endTime > 0) // 防止运行时进程未结束
        {
            totalServiceTime += p.serviceTime;
            totalResponseTime += p.responseTime;
            totalTurnaroundTime += p.turnaroundTime;
            totalWeightedTurnaroundTime += p.weightedTurnaroundTime;
            totalWaitTime += p.waitTime;
        }
        else
        {
            cout << "错误: 进程 " << p.id << " 未结束运行!!!" << endl;
        }
    }

    Utilization = 100 * totalServiceTime / processes.back().endTime;

    cout << "平均响应时间：" << totalResponseTime / n << endl;
    cout << "平均周转时间：" << totalTurnaroundTime / n << endl;
    cout << "平均带权周转时间：" << totalWeightedTurnaroundTime / n << endl;
    cout << "平均等待时间：" << totalWaitTime / n << endl;
    cout << "利用率：" << Utilization << " %" << endl;
    cout << "计算完成" << endl;
}

// 时间片轮转调度
void roundRobinScheduling(vector<Process> &processes, double timeSlice)
{
    cout << "时间片轮转调度开始" << endl;

    int n = processes.size();
    // 没有进程信息则返回
    if (n == 0)
    {
        cout << "vector中暂无进程信息" << endl;
        return;
    }

    vector<Process> complete;  // 已完成进程
    queue<Process> readyQueue; // 就绪队列
    Process p;
    double time = 0;   // 当前时间
    int count = 0;     // 就绪的进程数量
    int completed = 0; // 已完成的进程数量

    /* readyQueue.push(p); // 将第一个进程放入就绪队列
    // 将与第一个进程同时就绪的进程放入就绪队列
    for (int i = 1; i < n; i++)
    {
        p = processes[i];
        if (p.arrivalTime > time)
        {
            break;
        }
        readyQueue.push(p);
        count++;
        cout << "time: " << time << "  \t进程 " << p.id << " 加入就绪队列" << endl;
    }*/

    // 时间片轮转
    while (completed < n)
    {
        while (count < n)
        {
            // 将进程加入就绪队列
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                readyQueue.push(q);
                count++;
                cout << "time: " << time << "  \t进程 " << q.id << " 已经加入就绪队列" << endl;
            }
            else
            {
                break;
            }
        }

        if (!readyQueue.empty())
        // 从就绪队列中取出进程
        {
            p = readyQueue.front();
            readyQueue.pop();

            // 进程开始时间未设置过则此时为进程开始时间
            if (p.startTime < 0)
            {
                p.startTime = time;
            }
            cout << "time: " << time << "  \t进程 " << p.id << " 开始运行" << endl;

            // 进程剩余时间大于时间片
            if (p.remainTime > timeSlice)
            {
                time += timeSlice;
                p.remainTime -= timeSlice;
            }
            // 进程将在此轮结束
            else
            {
                time += p.remainTime;
                p.remainTime = 0;
                p.endTime = time;
                p.calculateTime();
                completed++;

                cout << "time: " << time << "  \t进程 " << p.id << " 完成" << endl;
            }

            // 将进程放回
            if (p.remainTime > 0)
            {
                readyQueue.push(p);
            }
            else
            {
                complete.push_back(p);
            }
        }
        else // 如果队列内没有进程，则跳过时间到最近的进程就绪
        {
            time = processes[count].arrivalTime;
        }
    }

    cout << "时间片轮转调度完成" << endl;

    performanceIndicators(complete);
}

// 短进程优先调度
void shortJobFirstScheduling(vector<Process> &processes)
{
    cout << "短进程优先调度开始" << endl;

    int n = processes.size();

    // 没有进程信息则返回
    if (n == 0)
    {
        cout << "vector中暂无进程信息" << endl;
        return;
    }

    vector<Process> complete; // 就绪队列
    Process p;
    double time = 0; // 当前时间
    int count = 0;   // 就绪的进程数量

    // complete.push_back(p); // 将第一个进程放入就绪队列

    // 短进程优先
    for (int i = 0; i < n;) // 考虑到此次循环可能存在无就绪进程的情况，将i++移至完成进程后
    {
        while (count < n)
        {
            // 将进程加入就绪队列
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                complete.push_back(q);
                count++;
                cout << "time: " << time << "  \t进程 " << q.id << " 已经加入就绪队列" << endl;
            }
            else
            {
                break;
            }
        }

        if (count > i) // 若存在就绪进程
        {
            // 将未完成进程按服务时间排序
            sort(complete.begin() + i, complete.end(), compareServiceTime);

            p = complete[i];
            if (p.arrivalTime > time)
            {
                time = p.arrivalTime;
            }
            p.startTime = time;
            time += p.serviceTime;
            p.endTime = time;
            p.remainTime = 0;
            p.calculateTime();
            complete[i] = p; // 将完成的进程数据传入到complete中
            cout << "进程 " << p.id << " 开始时间：" << p.startTime << "\t结束时间：" << p.endTime << endl;
            i++;
        }
        else
        {
            time = processes[count].arrivalTime;
        }
    }

    cout << "短进程优先调度完成" << endl;

    performanceIndicators(complete);
}

// 多级反馈队列调度
void multiLevelFeedbackQueueScheduling(vector<Process> &processes, double timeSlice0, double timeSlice1, double timeSlice2)
{
    cout << "多级反馈队列调度开始" << endl;

    int n = processes.size();

    // 没有进程信息则返回
    if (n == 0)
    {
        cout << "vector中暂无进程信息" << endl;
        return;
    }

    vector<Process> complete;                                  // 已完成进程
    vector<queue<Process>> readyQueue;                         // 多级反馈队列
    double timeSlice[] = {timeSlice0, timeSlice1, timeSlice2}; // 时间片
    queue<Process> readyQueue0, readyQueue1, readyQueue2;      // 就绪队列

    readyQueue.push_back(readyQueue0); // 将就绪队列依次放入vector
    readyQueue.push_back(readyQueue1);
    readyQueue.push_back(readyQueue2);
    int qnum = readyQueue.size() - 1;

    Process p, thisTurn;
    bool back = 0;     // 是否回溯
    double time = 0;   // 当前时间
    int count = 0;     // 就绪的进程数量
    int completed = 0; // 已完成的进程数量

    while (completed < n)
    {
        for (int i = 0; i <= qnum; i++)
        {
            if (!readyQueue[i].empty())
            {
                // 从最小的就绪队列中取出进程
                p = readyQueue[i].front();
                readyQueue[i].pop();

                // 进程开始时间未设置过则此时为进程开始时间
                if (p.startTime < 0)
                {
                    p.startTime = time;
                }
                cout << "time: " << time << "  \t进程 " << p.id << " 在 " << i + 1 << " 级队列开始运行" << endl;

                // 进程剩余时间大于时间片
                if (p.remainTime > timeSlice[i])
                {
                    time += timeSlice[i];
                    p.remainTime -= timeSlice[i];
                }
                // 进程将在此轮结束
                else
                {
                    time += p.remainTime;
                    p.remainTime = 0;
                    p.endTime = time;
                    p.calculateTime();
                    completed++;

                    cout << "time: " << time << "  \t进程 " << p.id << " 完成" << endl;
                }

                thisTurn = p; // 临时存储当前运行的进程

                break;
            }
            else if (i >= qnum) // 如果三级队列都没有进程，则跳过时间到最近的进程就绪
            {
                time = processes[count].arrivalTime;
            }
        }

        while (count < n)
        {
            // 将进程加入就绪队列
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                q.priority = 0;
                readyQueue[0].push(q);
                count++;
                cout << "time: " << time << "  \t进程 " << q.id << " 已经加入就绪队列" << endl;

                // 如果新就绪的进程到达时间早于当前时间，且此次运行的程序不在一级队列，则需要考虑抢占
                if (q.arrivalTime < time && q.priority < thisTurn.priority)
                {
                    if (thisTurn.remainTime == 0)
                    {
                        completed--; // 完成进程数量减一
                    }

                    back = 1;                            // 回溯信号置一
                    int backTime = time - q.arrivalTime; // 需要回溯的时间
                    time = q.arrivalTime;                // 时间回溯
                    thisTurn.remainTime += backTime;     // 剩余时间回溯
                    thisTurn.endTime = -1;               // 结束时间重置

                    cout << "进程 " << thisTurn.id << " 回溯: " << backTime << " ,剩余时间 " << thisTurn.remainTime << endl;
                }
            }
            else
            {
                break;
            }
        }

        // 将进程放回
        if (thisTurn.remainTime > 0)
        {
            if (thisTurn.priority < qnum && !back)
            {
                thisTurn.priority++;
            }
            readyQueue[thisTurn.priority].push(thisTurn);
            cout << "进程 " << thisTurn.id << " 进入队列 " << thisTurn.priority + 1 << endl;
        }
        else if (thisTurn.remainTime == 0)
        {
            complete.push_back(thisTurn);
        }
        thisTurn.remainTime = -1; // 防止重复放入
        back = 0;                 // 回溯信号归零
    }

    cout << "多级反馈队列调度完成" << endl;

    performanceIndicators(complete);
}

int main()
{
    vector<Process> processes;

    if (readFile(processes))
    {

        cout << "读取进程成功" << endl;

        while (1)
        {
            // 选择调度算法
            cout << "请选择调度算法：" << endl;
            cout << "1.时间片轮转调度" << endl;
            cout << "2.短进程优先调度" << endl;
            cout << "3.多级反馈队列调度" << endl;
            cout << "请输入您的选择：" << endl;
            int choice = 0;
            cin >> choice;
            switch (choice)
            {
            case 1:
                roundRobinScheduling(processes, 4);
                break;
            case 2:
                shortJobFirstScheduling(processes);
                break;
            case 3:
                multiLevelFeedbackQueueScheduling(processes, 1, 2, 4);
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
        cout << "读取进程失败" << endl;
    }

    // system("pause");
    return 0;
}