#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

int findHit(vector<int>& tempCol, int refPage)
{

	int hitIndex;

	for (int i = 0; i < tempCol.size(); i++)
	{
		if (tempCol.at(i) == refPage)
		{
			hitIndex = i;
			break;
		}
	}

	return hitIndex;
}

void pop_front(vector<int>& vec)
{
	reverse(vec.begin(), vec.end());
	vec.pop_back();
	reverse(vec.begin(), vec.end());

}

void drawMemory(vector<vector<int>>& memory, int cols, ofstream& raport)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout << memory.at(j).at(i) << "\t";
			raport << memory.at(j).at(i) << "\t";
		}
		cout << endl;
		raport << endl;
	}
}

void resetMemory(vector<vector<int>>& memory, vector<int>& reference, vector<int>& tempCol, int cols)
{

	for (int i = 0; i < cols; i++)
	{
		vector<int> col;
		for (int j = 0; j < 3; j++)
		{
			int temp = -1;
			col.push_back(temp);

		}
		memory.at(i) = col;
	}

	tempCol = memory.at(0);

	cout << endl;
}

int findEmptyIndex(vector<int>& tempCol) {

	int emptyIndex;

	for (int i = 0; i < tempCol.size(); i++)
	{
		if (tempCol.at(i) == -1)
		{
			emptyIndex = i;
			break;
		}
	}

	return emptyIndex;
}

int checkSC(int pos, vector<vector<int>>& memory, vector<int>& chances, int secondChance)
{

	//cout << "secondchance: " << secondChance << endl;

	vector<int> checkQ{ 0, 0, 0 };

	for (int i = 0; i < 3; i++)
	{
		//cout << "chances[i]: " << chances.at(i) << endl;
		if (chances.at(i) == 0)
		{
			int check = memory.at(pos - 1).at(i);
			int chQ = 0;

			for (int j = pos - 1; j > 0; j--)
			{
				if (memory.at(j).at(i) == check)
				{
					chQ++;
					if (j == 0)
					{
						//checkQ.push_back(chQ);
						checkQ.at(i) = chQ;
					}
				}
				else if (memory.at(j).at(i) != check)
				{
					//checkQ.push_back(chQ);
					checkQ.at(i) = chQ;
					j = 0;
				}
			}
		}
	}

	int First = max_element(checkQ.begin(), checkQ.end()) - checkQ.begin();

	chances.at(secondChance) = 0;

	//cout << First;

	return First;
}

int checkFIFO(int pos, vector<vector<int>>& memory)
{
	vector<int> checkQ;

	for (int i = 0; i < 3; i++)
	{
		checkQ.push_back(0);
	}

	for (int i = 0; i < 3; i++)
	{
		int check = memory.at(pos - 1).at(i);
		int chQ = 0;

		for (int j = pos - 1; j >= 0; j--)
		{
			if (memory.at(j).at(i) == check)
			{
				chQ++;
				if (j == 0)
				{
					//checkQ.push_back(chQ);
					checkQ.at(i) = chQ;
				}
			}
			else if (memory.at(j).at(i) != check)
			{
				//checkQ.push_back(chQ);
				checkQ.at(i) = chQ;
				j = 0;
			}
		}
	}

	int First = max_element(checkQ.begin(), checkQ.end()) - checkQ.begin();

	//cout << First;

	return First;
}

int checkLRU(int pos, vector<int>& tempCol, vector<int>& reference)
{
	vector<int> checks{0, 0, 0};

	int least = -1;

	int leastIndex;

	for (int i = pos - 1; i >= 0; i--)
	{
		for (int j = 0; j < tempCol.size(); j++)
		{
			if (tempCol.at(j) == reference.at(i))
			{
				checks.at(j)++;
				if (checks.at(0) > 0 && checks.at(1) > 0 && checks.at(2) > 0)
				{
					least = reference.at(i);
				}
			}
		}
		if (least != -1)
		{
			break;
		}
	}

	for (int k = 0; k < tempCol.size(); k++)
	{
		if (least == tempCol.at(k))
		{
			leastIndex = k;
		}
	}

	//cout << "\nlest: " << least << "\nIndex: " << leastIndex << "\n";

	return leastIndex;

}

int checkOPT(int pos, vector<int>& tempCol, vector<int>& reference, vector<vector<int>>& memory)
{
	vector<int> distances{ 0, 0, 0 };
	vector<int> tempRefs = reference;

	for (int i = 0; i < pos + 1; i++)
	{
		pop_front(tempRefs);
	}

	int counter = 0;

	int furthest = -1;

	int furthestIndex;

	for (int i = 0; i < tempCol.size(); i++)
	{
		if (find(tempRefs.begin(), tempRefs.end(), tempCol.at(i)) != tempRefs.end())
		{
			for (int j = 0; j < tempRefs.size(); j++)
			{
				if (tempCol.at(i) != tempRefs.at(j))
				{
					counter++;
				}
				else
				{
					distances.at(i) = counter;
					counter = 0;
					break;
				}
			}
		}
		else
		{
			distances.at(i) = -1;
		}
	}

	counter = 0;

	for (int i = 0; i < distances.size(); i++)
	{
		if (distances.at(i) == -1)
		{
			counter++;
		}
	}

	switch (counter)
	{
	case 0:
		furthestIndex = max_element(distances.begin(), distances.end()) - distances.begin();
		break;
	case 1:
		for (int i = 0; i < distances.size(); i++)
		{
			if (distances.at(i) == -1)
			{
				furthestIndex = i;
			}
		}
		break;
	case 2:
		furthestIndex = checkFIFO(pos, memory);
		//furthestIndex = 0;
		break;
	case 3:
		furthestIndex = checkFIFO(pos, memory);
		//furthestIndex = 0;
		break;
	default:
		break;
	}

	

	//cout << "\nlest: " << least << "\nIndex: " << leastIndex << "\n";

	return furthestIndex;
}

void OPT(vector<int>& tempCol, vector<int>& reference, vector<vector<int>>& memory, ofstream& raport)
{
	cout << "\nOPT\n";
	raport << "\nOPT\n";

	double pageHits = 0;
	double pageFaults = 0;

	vector<string> memoryStates;
	for (int i = 0; i < reference.size(); i++)
	{
		memoryStates.push_back("x");
	}

	int cols = reference.size();

	for (int i = 0; i < cols; i++)
	{
		int refPage = reference.at(i);

		if (find(tempCol.begin(), tempCol.end(), refPage) != tempCol.end())		//page hit
		{
			//cout << "page hit\t";
			memory.at(i) = tempCol;
			pageHits++;
			memoryStates.at(i) = "H";
		}
		else	//page fault
		{
			memory.at(i) = tempCol;
			pageFaults++;
			memoryStates.at(i) = "F";

			if (find(tempCol.begin(), tempCol.end(), -1) != tempCol.end())
			{
				int emptyIndex = findEmptyIndex(tempCol);
				memory.at(i).at(emptyIndex) = refPage;
				tempCol = memory.at(i);
			}
			else
			{
				memory.at(i) = tempCol;
				int indexToChange = checkOPT(i, tempCol, reference, memory);
				memory.at(i).at(indexToChange) = refPage;
				tempCol = memory.at(i);
				//cout << "page fault\t";
			}
		}
	}

	for (int i = 0; i < reference.size(); i++)
	{
		cout << reference.at(i) << "\t";
	}

	cout << endl << endl;
	drawMemory(memory, cols, raport);
	cout << endl;

	for (int i = 0; i < memoryStates.size(); i++)
	{
		cout << memoryStates.at(i) << "\t";
		raport << memoryStates.at(i) << "\t";
	}

	cout << endl;

	cout << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits/memoryStates.size()) * 100 << "%" << endl;
	raport << endl;

	raport << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
}

void FIFO(vector<int>& tempCol, vector<int>& reference, vector<vector<int>>& memory, ofstream& raport)
{
	cout << "\nFIFO\n";
	raport << "\nFIFO\n";

	double pageHits = 0;
	double pageFaults = 0;

	vector<string> memoryStates;
	for (int i = 0; i < reference.size(); i++)
	{
		memoryStates.push_back("x");
	}

	int cols = reference.size();

	for (int i = 0; i < cols; i++)
	{
		int refPage = reference.at(i);

		if (find(tempCol.begin(), tempCol.end(), refPage) != tempCol.end())		//page hit
		{
			//cout << "page hit\t";
			memory.at(i) = tempCol;
			pageHits++;
			memoryStates.at(i) = "H";
		}
		else	//page fault
		{
			memory.at(i) = tempCol;
			pageFaults++;
			memoryStates.at(i) = "F";

			if (find(tempCol.begin(), tempCol.end(), -1) != tempCol.end())
			{
				int emptyIndex = findEmptyIndex(tempCol);
				memory.at(i).at(emptyIndex) = refPage;
				tempCol = memory.at(i);
			}
			else
			{
				int indexToChange = checkFIFO(i, memory);
				memory.at(i).at(indexToChange) = refPage;
				tempCol = memory.at(i);
				//cout << "page fault\t";
			}
		}
	}

	for (int i = 0; i < reference.size(); i++)
	{
		cout << reference.at(i) << "\t";
	}

	cout << endl << endl;
	drawMemory(memory, cols, raport);
	cout << endl;

	for (int i = 0; i < memoryStates.size(); i++)
	{
		cout << memoryStates.at(i) << "\t";
		raport << memoryStates.at(i) << "\t";
	}

	cout << endl;

	cout << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
	raport << endl;

	raport << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
}

void LRU(vector<int>& tempCol, vector<int>& reference, vector<vector<int>>& memory, ofstream& raport)
{
	cout << "\nLRU\n";
	raport << "\nLRU\n";

	double pageHits = 0;
	double pageFaults = 0;

	vector<string> memoryStates;
	for (int i = 0; i < reference.size(); i++)
	{
		memoryStates.push_back("x");
	}

	int cols = reference.size();

	for (int i = 0; i < cols; i++)
	{
		int refPage = reference.at(i);

		if (find(tempCol.begin(), tempCol.end(), refPage) != tempCol.end())		//page hit
		{
			//cout << "page hit\t";
			memory.at(i) = tempCol;
			pageHits++;
			memoryStates.at(i) = "H";
		}
		else	//page fault
		{
			memory.at(i) = tempCol;
			pageFaults++;
			memoryStates.at(i) = "F";

			if (find(tempCol.begin(), tempCol.end(), -1) != tempCol.end())
			{
				int emptyIndex = findEmptyIndex(tempCol);
				memory.at(i).at(emptyIndex) = refPage;
				tempCol = memory.at(i);
			}
			else
			{
				int indexToChange = checkLRU(i, tempCol, reference);
				memory.at(i).at(indexToChange) = refPage;
				tempCol = memory.at(i);
				//cout << "page fault\t";
			}
		}
	}

	for (int i = 0; i < reference.size(); i++)
	{
		cout << reference.at(i) << "\t";
	}

	cout << endl << endl;
	drawMemory(memory, cols, raport);
	cout << endl;

	for (int i = 0; i < memoryStates.size(); i++)
	{
		cout << memoryStates.at(i) << "\t";
		raport << memoryStates.at(i) << "\t";
	}

	cout << endl;

	cout << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
	raport << endl;

	raport << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
}

void SC(vector<int>& tempCol, vector<int>& reference, vector<vector<int>>& memory, ofstream& raport)
{

	int indexToChange;
	int secondChance;

	cout << "\nSC\n";
	raport << "\nSC\n";

	double pageHits = 0;
	double pageFaults = 0;

	vector<string> memoryStates;
	for (int i = 0; i < reference.size(); i++)
	{
		memoryStates.push_back("x");
	}

	vector<int> chances{ 0, 0, 0 };

	int cols = reference.size();

	for (int i = 0; i < cols; i++)
	{
		int refPage = reference.at(i);

		if (find(tempCol.begin(), tempCol.end(), refPage) != tempCol.end())		//page hit
		{
			//cout << "page hit\t";
			memory.at(i) = tempCol;
			int hitIndex = findHit(tempCol, refPage);
			//cout << hitIndex << endl;
			chances.at(hitIndex) = 1;
			pageHits++;
			memoryStates.at(i) = "H";
		}
		else	//page fault
		{
			memory.at(i) = tempCol;
			pageFaults++;
			memoryStates.at(i) = "F";

			if (find(tempCol.begin(), tempCol.end(), -1) != tempCol.end())
			{
				int emptyIndex = findEmptyIndex(tempCol);
				memory.at(i).at(emptyIndex) = refPage;
				tempCol = memory.at(i);
			}
			else
			{
				int checkChances = 0;
				for (int j = 0; j < chances.size(); j++)
				{
					if (chances.at(j) == 1)
					{
						checkChances++;
					}
				}
				if (checkChances == 3 || checkChances == 0)
				{
					indexToChange = checkFIFO(i, memory);
					memory.at(i).at(indexToChange) = refPage;
					tempCol = memory.at(i);
				}
				else
				{
					secondChance = checkFIFO(i, memory);
					indexToChange = checkSC(i, memory, chances, secondChance);
					memory.at(i).at(indexToChange) = refPage;
					tempCol = memory.at(i);	
					secondChance = -1;
				}
				//cout << "page fault\t";
			}
		}
	}

	for (int i = 0; i < reference.size(); i++)
	{
		cout << reference.at(i) << "\t";
	}

	cout << endl << endl;
	drawMemory(memory, cols, raport);
	cout << endl;

	for (int i = 0; i < memoryStates.size(); i++)
	{
		cout << memoryStates.at(i) << "\t";
		raport << memoryStates.at(i) << "\t";
	}

	cout << endl;

	cout << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
	raport << endl;

	raport << "\nPage hits: " << pageHits << "\tPage faults: " << pageFaults << "\t% hits: " << (pageHits / memoryStates.size()) * 100 << "%" << endl;
}

int main()
{
	ofstream raport("raport.txt");

	vector<int> reference;

	string linia;
	ifstream plik;
	int tempRef;

	plik.open("dane.txt");

	while (!plik.eof()) 
	{
		getline(plik, linia);

		plik >> tempRef;
		reference.push_back(tempRef);
	}

	int cols = reference.size();

	vector<vector<int>> memory;

	for (int i = 0; i < cols; i++)
	{
		vector<int> col;
		for (int j = 0; j < 3; j++)
		{
			int temp = -1;
			col.push_back(temp);

		}
		memory.push_back(col);
	}

	//drawMemory(memory, cols);
	//cout << endl;
	vector<int> tempCol = memory.at(0);

	LRU(tempCol, reference, memory, raport);

	resetMemory(memory, reference, tempCol, cols);

	FIFO(tempCol, reference, memory, raport);

	resetMemory(memory, reference, tempCol, cols);

	OPT(tempCol, reference, memory, raport);

	resetMemory(memory, reference, tempCol, cols);

	SC(tempCol, reference, memory, raport);

	raport.close();

	cin.get();
}
