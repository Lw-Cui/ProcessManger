#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "manager.h"
#include "utility.h"
using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printInfo("You must specific input file");
		exit(1);
	}
	ifstream fin(argv[1]);
	string fullCommand;
	Manager manager;
	cout << manager.getCurPID() << endl;
	while (getline(fin, fullCommand)) {
		cout << fullCommand;
		istringstream sin(fullCommand);
		string command;
		sin >> command;
		if (command == "cr") {
			string pid;
			int priority;
			sin >> pid >> priority;
			manager.createProcess(pid, static_cast<Priority>(priority));
		}
		else if (command == "to") {
			manager.timeOut();
		}
		else if (command == "req") {
			string id;
			int num;
			sin >> id >> num;
			manager.requestResource(id, num);
		}
		else if (command == "rel") {
			string id;
			int num;
			sin >> id >> num;
			manager.releaseResource(id, num);
		}
		else if (command == "de") {
			string id;
			sin >> id;
			manager.destroy(id);
		}
		cout << "\t" << manager.getCurPID() << endl;
	}
	return 0;
}