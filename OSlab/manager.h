#pragma once
#include <string>
#include <list>
#include <unordered_set>
#include <vector>
#include <memory>
class ProcessControl;
using pPCB = std::shared_ptr<ProcessControl>;
using plist = std::list<pPCB>::iterator;
constexpr int RESOURCE = 4;

enum Priority {
	INIT = 0,
	USR = 1,
	SYS = 2,
};

class ResourceManger {
public:
	ResourceManger(int init);
	int getAvailableNum();
	void pushBackToWaiting(pPCB ptr);
private:
	int initNum;
	int available;
	std::list<pPCB> WaitingList;
};

class ScheduleManger {
public:
	ScheduleManger();
	plist pushBack(pPCB ptr);
	void remove(pPCB ptr);
	void request(pPCB ptr, int res, int num);
	void release(pPCB ptr, int res, int num);
	pPCB schedule();
private:
	std::vector<std::list<pPCB>> readyList;
	std::vector<ResourceManger> resouce;
};

class ProcessControl {
public:
	Priority getPriority();
	ProcessControl(const std::string &id, Priority p);
private:
	friend class Manager;
	std::string pid;
	Priority priority;
	pPCB parent;
	std::list<pPCB> children;
};

class Manager {
public:
	Manager();
	std::string getCurPID();
	void createProcess(const std::string &id, Priority p);
	void timeOut();
private:
	void schedule();
	pPCB runningProcess;
	ScheduleManger scheduler;
};