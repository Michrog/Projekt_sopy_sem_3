#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "process.h"
#include <iterator>
#include <Windows.h>
#include <math.h>
#include <ctime>
#include <cstdlib>

using namespace::std;

void addProcess(vector<process>& processes, int amount, int timeStamp, int position, int timeQ) 
{

	int nP = processes.size() + 1;
	process temp;
	int maxBurst = timeQ + 1;

	for (int i = 0; i < amount; i++)
	{

		string name = "P";
		name += to_string(nP + i);

		temp.arrival = timeStamp + rand() / ((RAND_MAX + 1u) / 10 + timeStamp);
		temp.burst = (1 + rand() / ((RAND_MAX + 1u) / maxBurst));
		temp.name = name;
		temp.start = 0;
		temp.waiting = 0;
		temp.complition = 0;

		processes.push_back(temp);

	}

	for (int i = position; i < processes.size(); i++)
	{
		for (int j = i + 1; j < processes.size(); j++)
		{
			if (processes.at(i).arrival > processes.at(j).arrival || (processes.at(i).arrival == processes.at(j).arrival && processes.at(i).burst > processes.at(j).burst))
			{
				temp = processes.at(j);
				processes.at(j) = processes.at(i);
				processes.at(i) = temp;
			}
		}
	}
	
}

void sortProcesses(vector<process>& processes)
{
	process temp;

	for (int i = 0; i < processes.size(); i++)
	{
		processes.at(i).complition = 0;
		processes.at(i).start = 0;
		processes.at(i).waiting = 0;
		for (int j = i + 1; j < processes.size(); j++)
		{
			if (processes.at(i).arrival > processes.at(j).arrival || (processes.at(i).arrival == processes.at(j).arrival && processes.at(i).burst > processes.at(j).burst)) 
			{
				temp = processes.at(j);
				processes.at(j) = processes.at(i);
				processes.at(i) = temp;
			}
		}
	}
}

void printSpaces(int x) 
{
	for (int i = 0; i < x; i++)
	{
		cout << " ";
	}	
}

void printHashes(int x, int speed)
{
	for (int i = 0; i < x; i++)
	{
		cout << "#";
		Sleep(speed);
	}
}

void drawGanttSJF(vector<process>& processes, int wybor, int q)
{

	int processesToAdd = 0;
	int timeTick = 0;
	int timeTickCheck = 0;

	int proba = 1;
	string decyzja;

	//int counterTwo = 0;

	process temp;
	vector<int> nextQueue;
	int counter = processes.at(0).burst;

	if (wybor == 3)
	{
		for (int i = 0; i < processes.size() - 1; i++)
		{
			counter += temp.burst;
			for (int j = i + 1; j < processes.size(); j++)
			{
				if (counter > processes.at(j).arrival)
				{
					nextQueue.push_back(processes.at(j).burst);
				}


			}
			vector<int>::iterator shortestJob = min_element(nextQueue.begin(), nextQueue.end());
			int shortestJobIndex = distance(nextQueue.begin(), shortestJob) + i + 1;

			temp = processes.at(shortestJobIndex);
			processes.at(shortestJobIndex) = processes.at(i + 1);
			processes.at(i + 1) = temp;

			nextQueue.clear();
		}

		counter = 0;
		for (int i = 0; i < processes.size(); i++)
		{
			if (processes.at(i).arrival >= counter)
			{
				counter = processes.at(i).arrival;
			}
			processes.at(i).start = counter;
			processes.at(i).waiting = counter - processes.at(i).arrival;
			processes.at(i).complition = processes.at(i).waiting + processes.at(i).burst;
			counter += processes.at(i).burst;
		}

	}

	switch (wybor)
	{
	case 1:

		for (int i = 0; i < processes.size() - 1; i++)
		{
			counter += temp.burst;
			for (int j = i + 1; j < processes.size(); j++)
			{
				if (counter > processes.at(j).arrival)
				{
					nextQueue.push_back(processes.at(j).burst);
				}


			}
			vector<int>::iterator shortestJob = min_element(nextQueue.begin(), nextQueue.end());
			int shortestJobIndex = distance(nextQueue.begin(), shortestJob) + i + 1;

			temp = processes.at(shortestJobIndex);
			processes.at(shortestJobIndex) = processes.at(i + 1);
			processes.at(i + 1) = temp;

			nextQueue.clear();
		}
		break;

	case 2:

		for (int i = 0; i < processes.size(); i++)
		{

			processesToAdd = 0;
			timeTick = floor(counter / q);
			//cout << timeTick << "|";

			if (timeTick > timeTickCheck)
			{
				processesToAdd = timeTick - timeTickCheck;
				//cout << processesToAdd << "|";
				addProcess(processes, processesToAdd, counter, i + 1, q);
				//cout << timeTickCheck << "|";
				timeTickCheck = timeTick;
			}

			counter += temp.burst;
			for (int j = i + 1; j < processes.size(); j++)
			{
				if (counter > processes.at(j).arrival)
				{
					nextQueue.push_back(processes.at(j).burst);
				}


			}

			if (!nextQueue.empty())
			{
				vector<int>::iterator shortestJob = min_element(nextQueue.begin(), nextQueue.end());
				int shortestJobIndex = distance(nextQueue.begin(), shortestJob) + i + 1;

				temp = processes.at(shortestJobIndex);
				processes.at(shortestJobIndex) = processes.at(i + 1);
				processes.at(i + 1) = temp;

				nextQueue.clear();
			}
			
		}
		break;

	default:
		break;
	}

	if (wybor != 3)
	{
		counter = 0;
		for (int i = 0; i < processes.size(); i++)
		{
			cout << processes.at(i).name + ":\t ";
			if (processes.at(i).arrival < counter)
				printSpaces(counter);
			else
			{
				printSpaces(processes.at(i).arrival);
				counter = processes.at(i).arrival;
			}
			processes.at(i).start = counter;
			processes.at(i).waiting = counter - processes.at(i).arrival;
			processes.at(i).complition = processes.at(i).waiting + processes.at(i).burst;
			if (wybor == 1)
			{
				printHashes(processes.at(i).burst, 50);
			}
			else
			{
				printHashes(processes.at(i).burst, 10);
			}
			counter += processes.at(i).burst;
			//cout << counter;
			cout << endl;
		}
	}
	
}

void drawGanttFCFS(vector<process>& processes, int wybor, int q)
{

	int processesToAdd = 0;
	int timeTick = 0;
	int timeTickCheck = 0;

	int proba = 1;
	string decyzja;

	int counter = 0;

	switch (wybor)
	{
	case 1:
		for (int i = 0; i < processes.size(); i++)
		{
			cout << processes.at(i).name + ":\t";
			if (processes.at(i).arrival < counter)
				printSpaces(counter);
			else
			{
				printSpaces(processes.at(i).arrival);
				counter = processes.at(i).arrival;
			}
			processes.at(i).start = counter;
			processes.at(i).waiting = counter - processes.at(i).arrival;
			processes.at(i).complition = processes.at(i).waiting + processes.at(i).burst;
			printHashes(processes.at(i).burst, 50);
			counter += processes.at(i).burst;
			cout << endl;
		}
		break;
	case 2:
		for (int i = 0; i < processes.size(); i++)
		{
			processesToAdd = 0;
			timeTick = floor(counter / q);
			//cout << timeTick << "|";
		
			if (timeTick > timeTickCheck)
			{
				processesToAdd = timeTick - timeTickCheck;
				//cout << processesToAdd << "|";
				addProcess(processes, processesToAdd, counter + processes.at(i).burst, i + 1, q);
				//cout << timeTickCheck << "|";
				timeTickCheck = timeTick;
			}

			cout << processes.at(i).name + ":\t";
			if (processes.at(i).arrival < counter)
				printSpaces(counter);
			else
			{
				printSpaces(processes.at(i).arrival);
				counter = processes.at(i).arrival;
			}
			processes.at(i).start = counter;
			processes.at(i).waiting = counter - processes.at(i).arrival;
			processes.at(i).complition = processes.at(i).waiting + processes.at(i).burst;
			printHashes(processes.at(i).burst, 10);
			//cout << counter + processes.at(i).burst;
			counter += processes.at(i).burst;
			cout << endl;
		}
		break;
	case 3:
		for (int i = 0; i < processes.size(); i++)
		{
			processesToAdd = 0;
			timeTick = floor(counter / q);
			//cout << timeTick << "|";

			if (timeTick > timeTickCheck)
			{
				processesToAdd = timeTick - timeTickCheck;
				//cout << processesToAdd << "|";
				addProcess(processes, processesToAdd, counter + processes.at(i).burst, i + 1, q);
				//cout << timeTickCheck << "|";
				timeTickCheck = timeTick;
			}

			if (processes.at(i).arrival >= counter)
			{
				counter = processes.at(i).arrival;
			}
			processes.at(i).start = counter;
			processes.at(i).waiting = counter - processes.at(i).arrival;
			processes.at(i).complition = processes.at(i).waiting + processes.at(i).burst;
			//cout << counter + processes.at(i).burst;
			counter += processes.at(i).burst;
		}
		break;
	default:
		break;
	}
}

void drawProcesses(vector<process>& processes, ofstream& raport)
{
	
	cout << "NAME\t" << "AT\t" << "BT\t" << "ST\t" << "WT\t" << "CT\t" << endl;
	raport << "NAME\t" << "AT\t" << "BT\t" << "ST\t" << "WT\t" << "CT\t" << endl;

	cout << endl;

	for (int i = 0; i < processes.size(); i++)
	{
		raport << processes.at(i).name << ":\t" << processes.at(i).arrival << "\t" << processes.at(i).burst << "\t" << processes.at(i).start << "\t" << processes.at(i).waiting << "\t" << processes.at(i).complition << endl;
		cout << processes.at(i).name << ":\t" << processes.at(i).arrival << "\t" << processes.at(i).burst << "\t" << processes.at(i).start << "\t" << processes.at(i).waiting << "\t" << processes.at(i).complition << endl;
		//Sleep(100);
	}

	cout << endl;

	int avgWT = 0;
	int avgCT = 0;

	for (int i = 0; i < processes.size(); i++)
	{
		avgWT += processes.at(i).waiting;
		avgCT += processes.at(i).complition;
	}

	avgWT /= processes.size();
	avgCT /= processes.size();

	raport << "AVG:\t" << "-\t" << "-\t" << "-\t" << avgWT << "\t" << avgCT << "\t" << endl << endl;
	cout << "AVG:\t" << "-\t" << "-\t" << "-\t" << avgWT << "\t" << avgCT << "\t" << endl;

}

int main()
{

	ofstream raport("raport.txt");

	srand(time(NULL));

	int q;
	cout << "Ustal kwant czasu dla otwartej puli zadan: ";
	cin >> q;
	cout << endl;

	vector<process> processes;
	process temp;
	string name = "P";
	int nP = 1;
	int totalTime = 0;
	
	string linia;
	ifstream dane;

	dane.open("procesy.txt");

	if (dane.is_open())
	{
		while (!dane.eof()) {
			getline(dane, linia);
			dane >> temp.arrival >> temp.burst;
			temp.name = name + to_string(nP);
			nP++;
			processes.push_back(temp);
		}
	}

	dane.close();

	sortProcesses(processes);

	//ustalPriorytet(processes);

	for (int i = 0; i < processes.size(); i++) {
		cout << processes.at(i).name << " " << processes.at(i).arrival << " " << processes.at(i).burst << " "  << endl;
	}

	int wybor;
	int wybor2;

	do
	{
		cout << "\nWybierz algorytm: \n1. FCFS\n2. SJF\n3. Porownanie\n4. Wyjscie\n";
		cin >> wybor;

		switch (wybor)
		{
		case 1:

			cout << "\nPula zadan: \n1. zamkniieta\n2. otwarta\n";
			cin >> wybor2;

			switch (wybor2)
			{
			case 1:

				cout << "\n\t|FCFS - zamknieta pula zadan|\n" << endl;
				drawGanttFCFS(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);
				sortProcesses(processes);
				break;

			case 2:

				cout << "\n\t|SJF - otwarta pula zadan|\n" << endl;
				drawGanttFCFS(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);

					processes.clear();

					nP = 1;

					dane.open("procesy.txt");

					if (dane.is_open())
					{
						while (!dane.eof()) {
							getline(dane, linia);
							dane >> temp.arrival >> temp.burst;
							temp.name = name + to_string(nP);
							nP++;
							processes.push_back(temp);
						}
					}

					dane.close();

				sortProcesses(processes);
				break;

			default:
				break;
			}

			break;
		case 2:

			cout << "\nPula zadan: \n1. zamkniieta\n2. otwarta\n";
			cin >> wybor2;

			switch (wybor2)
			{
			case 1:

				cout << "\n\t|SJF - zamknieta pula zadan|\n" << endl;
				drawGanttSJF(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);
				sortProcesses(processes);
				break;

			case 2:

				cout << "\n\t|SJF - otwarta pula zadan|\n" << endl;
				drawGanttSJF(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);

				processes.clear();

				nP = 1;

				dane.open("procesy.txt");

				if (dane.is_open())
				{
					while (!dane.eof()) {
						getline(dane, linia);
						dane >> temp.arrival >> temp.burst;
						temp.name = name + to_string(nP);
						nP++;
						processes.push_back(temp);
					}
				}

				dane.close();

				sortProcesses(processes);
				break;

			default:
				break;
			}

			break;
		case 3:

			cout << "Porownanie dla puli zamknietej [1] czy otwartej [2]?: ";
			cin >> wybor2;

			switch (wybor2)
			{
			case 1:
				cout << "\n\t|Porownanie - zamknieta pula zadan|\n" << endl;

				drawGanttFCFS(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);
				sortProcesses(processes);

				cout << "\n-------------^FCFS^------------------------vSJFv-------------\n" << endl;
				raport << "-------------^FCFS^------------------------vSJFv-------------\n" << endl;

				drawGanttSJF(processes, wybor2, q);
				cout << endl;
				drawProcesses(processes, raport);
				sortProcesses(processes);

				break;
			case 2:
				cout << "\n\t|Porownanie - otwarta pula zadan|\n" << endl;

				drawGanttFCFS(processes, 3, q);
				cout << endl;
				drawProcesses(processes, raport);
				sortProcesses(processes);

				cout << "\n-------------^FCFS^------------------------vSJFv-------------\n" << endl;
				raport << "-------------^FCFS^------------------------vSJFv-------------\n" << endl;

				drawGanttSJF(processes, 3, q);
				cout << endl;
				drawProcesses(processes, raport);

				processes.clear();

				nP = 1;

				dane.open("procesy.txt");

				if (dane.is_open())
				{
					while (!dane.eof()) {
						getline(dane, linia);
						dane >> temp.arrival >> temp.burst;
						temp.name = name + to_string(nP);
						nP++;
						processes.push_back(temp);
					}
				}

				dane.close();

				sortProcesses(processes);

				break;
			default:
				break;
			}
			break;
		default:
			cout << "Nic nie wybrano" << endl;
			break;
		}

	} while (wybor != 4);

}
