#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

// ���ý�����
class Process
{
public:
    string id;                     // ����id
    double arrivalTime;            // ����ʱ��
    double serviceTime;            // ����ʱ��
    double priority;               // ���ȼ�
    double startTime;              // ��ʼʱ��
    double endTime;                // ����ʱ��
    double responseTime;           // ��Ӧʱ��
    double turnaroundTime;         // ��תʱ��
    double weightedTurnaroundTime; // ��Ȩ��תʱ��
    double waitTime;               // �ȴ�ʱ��
    double remainTime;             // ʣ��ʱ��

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

    // ������ɺ�������ʱ��
    void calculateTime()
    {
        if (startTime < 0)
        {
            cout << "���� " << id << " δ��ʼ����!!!" << endl;
            return;
        }
        if (endTime < 0)
        {
            cout << "���� " << id << " δ��������!!!" << endl;
            return;
        }

        responseTime = startTime - arrivalTime;                // ��Ӧʱ��=����ʱ��-��ʼʱ��
        turnaroundTime = endTime - arrivalTime;                // ��תʱ��=����ʱ��-����ʱ��
        weightedTurnaroundTime = turnaroundTime / serviceTime; // ��Ȩ��תʱ��=��תʱ��/����ʱ��
        waitTime = turnaroundTime - serviceTime;               // �ȴ�ʱ��=��תʱ��-����ʱ��
    }
};

// �ȽϷ���ʱ���С
bool compareServiceTime(Process a, Process b)
{
    return a.serviceTime < b.serviceTime;
}

// ��ȡ�洢�ļƻ�
bool readFile(vector<Process> &processes)
{
    ifstream ifs;
    ifs.open("xym1.txt", ios::in);

    // �ļ��������򷵻�
    if (!ifs)
    {
        cout << "�ļ�������" << endl;
        return 0;
    }

    // ���ļ��򷵻�
    if (ifs.peek() == ifstream::traits_type::eof())
    {
        cout << "û�н�����Ϣ" << endl;
        return 0;
    }

    cout << "��ȡ������Ϣ" << endl;
    cout << left << "|" << setw(10) << "����ID"
         << "|" << setw(10) << "����ʱ��"
         << "|" << setw(10) << "����ʱ��" << endl;

    // ��ȡ������Ϣ
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

// ��ʾ���
void showResult(vector<Process> &processes)
{
    int n = processes.size();

    cout << "��ʾ���" << endl;
    cout << left << "|" << setw(10) << "����ID"
         << "|" << setw(10) << "����ʱ��"
         << "|" << setw(10) << "����ʱ��"
         << "|" << setw(10) << "���ȼ�"
         << "|" << setw(10) << "��ʼʱ��"
         << "|" << setw(10) << "����ʱ��"
         << "|" << setw(10) << "��Ӧʱ��"
         << "|" << setw(10) << "��תʱ��"
         << "|" << setw(14) << "��Ȩ��תʱ��"
         << "|" << setw(10) << "�ȴ�ʱ��" << endl;

    for (int i = 0; i < n; i++)
    {
        Process p = processes[i];
        p.printResult();
    }
}

// ����ƽ����תʱ��/ƽ����Ȩ��תʱ�䣬ƽ���ȴ�ʱ�䡢ƽ����Ӧʱ�䡢�����ʵ�����ָ�꼰��ƽ��ָ��
void performanceIndicators(vector<Process> &processes)
{
    cout << "��ʼ��������ָ��" << endl;

    showResult(processes);

    Process p;
    int n = processes.size();               // ��������
    double totalServiceTime = 0;            //
    double totalResponseTime = 0;           // ����Ӧʱ��
    double totalTurnaroundTime = 0;         // ����תʱ��
    double totalWeightedTurnaroundTime = 0; // �ܴ�Ȩ��תʱ��
    double totalWaitTime = 0;               // �ܵȴ�ʱ��
    double Utilization = 0;                 // ������

    for (int i = 0; i < n; i++)
    {
        p = processes[i];
        if (p.endTime > 0) // ��ֹ����ʱ����δ����
        {
            totalServiceTime += p.serviceTime;
            totalResponseTime += p.responseTime;
            totalTurnaroundTime += p.turnaroundTime;
            totalWeightedTurnaroundTime += p.weightedTurnaroundTime;
            totalWaitTime += p.waitTime;
        }
        else
        {
            cout << "����: ���� " << p.id << " δ��������!!!" << endl;
        }
    }

    Utilization = 100 * totalServiceTime / processes.back().endTime;

    cout << "ƽ����Ӧʱ�䣺" << totalResponseTime / n << endl;
    cout << "ƽ����תʱ�䣺" << totalTurnaroundTime / n << endl;
    cout << "ƽ����Ȩ��תʱ�䣺" << totalWeightedTurnaroundTime / n << endl;
    cout << "ƽ���ȴ�ʱ�䣺" << totalWaitTime / n << endl;
    cout << "�����ʣ�" << Utilization << " %" << endl;
    cout << "�������" << endl;
}

// ʱ��Ƭ��ת����
void roundRobinScheduling(vector<Process> &processes, double timeSlice)
{
    cout << "ʱ��Ƭ��ת���ȿ�ʼ" << endl;

    int n = processes.size();
    // û�н�����Ϣ�򷵻�
    if (n == 0)
    {
        cout << "vector�����޽�����Ϣ" << endl;
        return;
    }

    vector<Process> complete;  // ����ɽ���
    queue<Process> readyQueue; // ��������
    Process p;
    double time = 0;   // ��ǰʱ��
    int count = 0;     // �����Ľ�������
    int completed = 0; // ����ɵĽ�������

    /* readyQueue.push(p); // ����һ�����̷����������
    // �����һ������ͬʱ�����Ľ��̷����������
    for (int i = 1; i < n; i++)
    {
        p = processes[i];
        if (p.arrivalTime > time)
        {
            break;
        }
        readyQueue.push(p);
        count++;
        cout << "time: " << time << "  \t���� " << p.id << " �����������" << endl;
    }*/

    // ʱ��Ƭ��ת
    while (completed < n)
    {
        while (count < n)
        {
            // �����̼����������
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                readyQueue.push(q);
                count++;
                cout << "time: " << time << "  \t���� " << q.id << " �Ѿ������������" << endl;
            }
            else
            {
                break;
            }
        }

        if (!readyQueue.empty())
        // �Ӿ���������ȡ������
        {
            p = readyQueue.front();
            readyQueue.pop();

            // ���̿�ʼʱ��δ���ù����ʱΪ���̿�ʼʱ��
            if (p.startTime < 0)
            {
                p.startTime = time;
            }
            cout << "time: " << time << "  \t���� " << p.id << " ��ʼ����" << endl;

            // ����ʣ��ʱ�����ʱ��Ƭ
            if (p.remainTime > timeSlice)
            {
                time += timeSlice;
                p.remainTime -= timeSlice;
            }
            // ���̽��ڴ��ֽ���
            else
            {
                time += p.remainTime;
                p.remainTime = 0;
                p.endTime = time;
                p.calculateTime();
                completed++;

                cout << "time: " << time << "  \t���� " << p.id << " ���" << endl;
            }

            // �����̷Ż�
            if (p.remainTime > 0)
            {
                readyQueue.push(p);
            }
            else
            {
                complete.push_back(p);
            }
        }
        else // ���������û�н��̣�������ʱ�䵽����Ľ��̾���
        {
            time = processes[count].arrivalTime;
        }
    }

    cout << "ʱ��Ƭ��ת�������" << endl;

    performanceIndicators(complete);
}

// �̽������ȵ���
void shortJobFirstScheduling(vector<Process> &processes)
{
    cout << "�̽������ȵ��ȿ�ʼ" << endl;

    int n = processes.size();

    // û�н�����Ϣ�򷵻�
    if (n == 0)
    {
        cout << "vector�����޽�����Ϣ" << endl;
        return;
    }

    vector<Process> complete; // ��������
    Process p;
    double time = 0; // ��ǰʱ��
    int count = 0;   // �����Ľ�������

    // complete.push_back(p); // ����һ�����̷����������

    // �̽�������
    for (int i = 0; i < n;) // ���ǵ��˴�ѭ�����ܴ����޾������̵��������i++������ɽ��̺�
    {
        while (count < n)
        {
            // �����̼����������
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                complete.push_back(q);
                count++;
                cout << "time: " << time << "  \t���� " << q.id << " �Ѿ������������" << endl;
            }
            else
            {
                break;
            }
        }

        if (count > i) // �����ھ�������
        {
            // ��δ��ɽ��̰�����ʱ������
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
            complete[i] = p; // ����ɵĽ������ݴ��뵽complete��
            cout << "���� " << p.id << " ��ʼʱ�䣺" << p.startTime << "\t����ʱ�䣺" << p.endTime << endl;
            i++;
        }
        else
        {
            time = processes[count].arrivalTime;
        }
    }

    cout << "�̽������ȵ������" << endl;

    performanceIndicators(complete);
}

// �༶�������е���
void multiLevelFeedbackQueueScheduling(vector<Process> &processes, double timeSlice0, double timeSlice1, double timeSlice2)
{
    cout << "�༶�������е��ȿ�ʼ" << endl;

    int n = processes.size();

    // û�н�����Ϣ�򷵻�
    if (n == 0)
    {
        cout << "vector�����޽�����Ϣ" << endl;
        return;
    }

    vector<Process> complete;                                  // ����ɽ���
    vector<queue<Process>> readyQueue;                         // �༶��������
    double timeSlice[] = {timeSlice0, timeSlice1, timeSlice2}; // ʱ��Ƭ
    queue<Process> readyQueue0, readyQueue1, readyQueue2;      // ��������

    readyQueue.push_back(readyQueue0); // �������������η���vector
    readyQueue.push_back(readyQueue1);
    readyQueue.push_back(readyQueue2);
    int qnum = readyQueue.size() - 1;

    Process p, thisTurn;
    bool back = 0;     // �Ƿ����
    double time = 0;   // ��ǰʱ��
    int count = 0;     // �����Ľ�������
    int completed = 0; // ����ɵĽ�������

    while (completed < n)
    {
        for (int i = 0; i <= qnum; i++)
        {
            if (!readyQueue[i].empty())
            {
                // ����С�ľ���������ȡ������
                p = readyQueue[i].front();
                readyQueue[i].pop();

                // ���̿�ʼʱ��δ���ù����ʱΪ���̿�ʼʱ��
                if (p.startTime < 0)
                {
                    p.startTime = time;
                }
                cout << "time: " << time << "  \t���� " << p.id << " �� " << i + 1 << " �����п�ʼ����" << endl;

                // ����ʣ��ʱ�����ʱ��Ƭ
                if (p.remainTime > timeSlice[i])
                {
                    time += timeSlice[i];
                    p.remainTime -= timeSlice[i];
                }
                // ���̽��ڴ��ֽ���
                else
                {
                    time += p.remainTime;
                    p.remainTime = 0;
                    p.endTime = time;
                    p.calculateTime();
                    completed++;

                    cout << "time: " << time << "  \t���� " << p.id << " ���" << endl;
                }

                thisTurn = p; // ��ʱ�洢��ǰ���еĽ���

                break;
            }
            else if (i >= qnum) // ����������ж�û�н��̣�������ʱ�䵽����Ľ��̾���
            {
                time = processes[count].arrivalTime;
            }
        }

        while (count < n)
        {
            // �����̼����������
            Process q = processes[count];
            if (q.arrivalTime <= time)
            {
                q.priority = 0;
                readyQueue[0].push(q);
                count++;
                cout << "time: " << time << "  \t���� " << q.id << " �Ѿ������������" << endl;

                // ����¾����Ľ��̵���ʱ�����ڵ�ǰʱ�䣬�Ҵ˴����еĳ�����һ�����У�����Ҫ������ռ
                if (q.arrivalTime < time && q.priority < thisTurn.priority)
                {
                    if (thisTurn.remainTime == 0)
                    {
                        completed--; // ��ɽ���������һ
                    }

                    back = 1;                            // �����ź���һ
                    int backTime = time - q.arrivalTime; // ��Ҫ���ݵ�ʱ��
                    time = q.arrivalTime;                // ʱ�����
                    thisTurn.remainTime += backTime;     // ʣ��ʱ�����
                    thisTurn.endTime = -1;               // ����ʱ������

                    cout << "���� " << thisTurn.id << " ����: " << backTime << " ,ʣ��ʱ�� " << thisTurn.remainTime << endl;
                }
            }
            else
            {
                break;
            }
        }

        // �����̷Ż�
        if (thisTurn.remainTime > 0)
        {
            if (thisTurn.priority < qnum && !back)
            {
                thisTurn.priority++;
            }
            readyQueue[thisTurn.priority].push(thisTurn);
            cout << "���� " << thisTurn.id << " ������� " << thisTurn.priority + 1 << endl;
        }
        else if (thisTurn.remainTime == 0)
        {
            complete.push_back(thisTurn);
        }
        thisTurn.remainTime = -1; // ��ֹ�ظ�����
        back = 0;                 // �����źŹ���
    }

    cout << "�༶�������е������" << endl;

    performanceIndicators(complete);
}

int main()
{
    vector<Process> processes;

    if (readFile(processes))
    {

        cout << "��ȡ���̳ɹ�" << endl;

        while (1)
        {
            // ѡ������㷨
            cout << "��ѡ������㷨��" << endl;
            cout << "1.ʱ��Ƭ��ת����" << endl;
            cout << "2.�̽������ȵ���" << endl;
            cout << "3.�༶�������е���" << endl;
            cout << "����������ѡ��" << endl;
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

    // system("pause");
    return 0;
}