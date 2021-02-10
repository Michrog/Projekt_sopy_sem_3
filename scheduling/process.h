#pragma once
#include <string>

class process {
	public:
		process() {

		}

		std::string name = "P";
		int arrival = 0;
		int burst = 0;
		int start = 0;
		int complition;
		int waiting;
};