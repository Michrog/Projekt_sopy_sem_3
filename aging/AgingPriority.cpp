#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <Windows.h>
#include <iterator>

using namespace std;

class process 
{
public:
	string name = "P";
	int AT = 0;
	int BT = 0;
	int ST = 0;
	int WT;
	int CT;
	int PRI = 0;
};

void sortProcesses(vector<process>& processes)
{
	process temp;

	for (int i = 0; i < processes.size(); i++)
	{
		processes.at(i).CT = 0;
		processes.at(i).ST = 0;
		processes.at(i).WT = 0;
		for (int j = i + 1; j < processes.size(); j++)
		{
			if (processes.at(i).AT > processes.at(j).AT || (processes.at(i).AT == processes.at(j).AT && processes.at(i).BT > processes.at(j).BT))
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

void printHashes(int x)
{
	for (int i = 0; i < x; i++)
	{
		cout << "#";
		Sleep(50);
	}
}

void aging(vector<process>& processes, int timeQ) 
{
	process temp;
	process temp2;
	vector<process> nextQueue;
	int counter = processes.at(0).BT;
	int ageTick = 0;
	int ageTickCheck = 0;
	int PRIpenalty = 0;

	for (int i = 0; i < processes.size() - 1; i++)
	{
		//cout << PRIpenalty;
		PRIpenalty = 0;
		//counter += temp.BT;
		ageTick = floor(counter / timeQ);

		//cout << ageTick << ageTickCheck << endl;

		for (int j = i + 1; j < processes.size(); j++)
		{
			if (counter > processes.at(j).AT)
			{
				if (ageTick > ageTickCheck)
				{
					PRIpenalty = ageTick - ageTickCheck;
				}
				nextQueue.push_back(processes.at(j));
			}
			ageTickCheck = ageTick;
		}

		sortProcesses(nextQueue);

		for (int n = 0; n < nextQueue.size(); n++)
		{
			nextQueue.at(n).PRI -= PRIpenalty;
			if (nextQueue.at(n).PRI < 0)
			{
				nextQueue.at(n).PRI = 0;
			}
		}

		//temp2 = nextQueue.at(0);

		for (int n = 0; n < nextQueue.size(); n++)
		{
			for (int k = n + 1; k < nextQueue.size(); k++)
			{
				if (nextQueue.at(n).PRI > nextQueue.at(k).PRI || (nextQueue.at(n).PRI == nextQueue.at(k).PRI && nextQueue.at(n).AT > nextQueue.at(k).AT))
				{
					temp2 = nextQueue.at(k);
					nextQueue.at(k) = nextQueue.at(n);
					nextQueue.at(n) = temp2;
				}
			}
		}

		for (int n = 0; n < nextQueue.size(); n++)
		{
			if (counter > processes.at(n + i + 1).AT)
			{
				processes.at(n + i + 1) = nextQueue.at(n);
			}
		}

		nextQueue.clear();

		counter += processes.at(i + 1).BT;
	}

	counter = 0;

	for (int i = 0; i < processes.size(); i++)
	{
		cout << processes.at(i).name + ":\t ";
		if (processes.at(i).AT < counter) 
		{
			printSpaces(counter);
		}
		else
		{
			printSpaces(processes.at(i).AT);
			counter = processes.at(i).AT;
		}
		processes.at(i).ST = counter;
		processes.at(i).WT = counter - processes.at(i).AT;
		processes.at(i).CT = processes.at(i).WT + processes.at(i).BT;
		printHashes(processes.at(i).BT);
		counter += processes.at(i).BT;
		cout << endl;
	}
}

void drawProcesses(vector<process>& processes, ofstream& raport)
{
	cout << "NAME\t" << "AT\t" << "BT\t" << "ST\t" << "WT\t" << "CT\t" << "PRI\t" << endl;
	raport << "NAME\t" << "AT\t" << "BT\t" << "ST\t" << "WT\t" << "CT\t" << "PRI\t" << endl;

	cout << endl;

	for (int i = 0; i < processes.size(); i++)
	{
		cout << processes.at(i).name << ":\t" << processes.at(i).AT << "\t" << processes.at(i).BT << "\t" << processes.at(i).ST << "\t" << processes.at(i).WT << "\t" << processes.at(i).CT << "\t" << processes.at(i).PRI << endl;
		raport << processes.at(i).name << ":\t" << processes.at(i).AT << "\t" << processes.at(i).BT << "\t" << processes.at(i).ST << "\t" << processes.at(i).WT << "\t" << processes.at(i).CT << "\t" << processes.at(i).PRI << endl;
	}

	cout << endl;

	int avgWT = 0;
	int avgCT = 0;

	for (int i = 0; i < processes.size(); i++)
	{
		avgWT += processes.at(i).WT;
		avgCT += processes.at(i).CT;
	}

	avgWT /= processes.size();
	avgCT /= processes.size();

	cout << "AVG:\t" << "-\t" << "-\t" << "-\t" << avgWT << "\t" << avgCT << "\t" << endl;
	raport << "AVG:\t" << "-\t" << "-\t" << "-\t" << avgWT << "\t" << avgCT << "\t" << endl;

}

int main()
{
	ofstream raport("raport.txt");

	vector<process> processes;
	process temp;

	string line;
	ifstream file;
	int counter = 0;

	file.open("data.txt");

	while (!file.eof()) 
	{

		temp.name = "P";
		getline(file, line);
		file >> temp.AT >> temp.BT >> temp.PRI;
		temp.name += to_string(counter);
		counter++;

		processes.push_back(temp);

	}

	int timeQ;

	cout << "\tFCFS - PRIORYTETOWY\nWybierz kwant czasu: ";
	cin >> timeQ;
	cout << endl;

	sortProcesses(processes);

	//drawProcesses(processes);

	aging(processes, timeQ);

	cout << endl;

	drawProcesses(processes, raport);

	raport.close();

	cin.get();
	cin.get();
}
