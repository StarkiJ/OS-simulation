#include <iostream>
#include <vector>

using namespace std;

struct Process {
    int id; // 进程ID
    int arrivalTime; // 到达时间
    int burstTime; // 运行时间
    int priority; // 优先级
    int startTime; // 开始时间
    int finishTime; // 结束时间
    int turnaroundTime; // 周转时间
    float waitingTime; // 等待时间
    float responseTime; // 响应时间
};

// 时间片轮转调度算法
void roundRobinScheduling(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    vector<int> remainingTime(n); // 记录每个进程的剩余运行时间
    vector<int> currentTime(n, 0); // 记录每个进程的当前运行时间
    int currentTimeGlobal = 0; // 记录全局当前时间
    int completed = 0; // 记录完成的进程数量

    // 初始化剩余运行时间
    for (int i = 0; i < n; ++i) {
        remainingTime[i] = processes[i].burstTime;
    }

    // 时间片轮转
    while (completed < n) {
        bool done = true;

        // 遍历所有进程
        for (int i = 0; i < n; ++i) {
            // 如果进程还没有执行完
            if (remainingTime[i] > 0) {
                done = false;

                // 执行时间片
                if (remainingTime[i] > timeQuantum) {
                    currentTimeGlobal += timeQuantum;
                    currentTime[i] += timeQuantum;
                    remainingTime[i] -= timeQuantum;
                } else {
                    currentTimeGlobal += remainingTime[i];
                    currentTime[i] += remainingTime[i];
                    remainingTime[i] = 0;
                    completed++;

                    // 计算结束时间、周转时间、等待时间和响应时间
                    processes[i].finishTime = currentTimeGlobal;
                    processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
                    processes[i].responseTime = processes[i].startTime - processes[i].arrivalTime;
                }
            }
        }

        // 如果所有进程都执行完了，跳出循环
        if (done)
            break;
    }
}

int main() {
    // 生成进程数据集
    vector<Process> processes = {
        {1, 0, 6, 2},
        {2, 1, 4, 1},
        {3, 2, 7, 2},
        {4, 3, 3, 3},
        {5, 4, 5, 1}
    };

    // 设置时间片大小
    int timeQuantum = 2;

    // 调用时间片轮转调度算法
    roundRobinScheduling(processes, timeQuantum);

    // 输出结果
    cout << "Process ID\tStart Time\tFinish Time\tTurnaround Time\tWaiting Time\tResponse Time\n";
    for (const auto& process : processes) {
        cout << process.id << "\t\t" << process.startTime << "\t\t" << process.finishTime << "\t\t"
             << process.turnaroundTime << "\t\t" << process.waitingTime << "\t\t" << process.responseTime << endl;
    }

    system("pause");
    return 0;
}
