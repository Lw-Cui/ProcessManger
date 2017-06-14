#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <list>
#include <unordered_set>
#include <vector>
#include <memory>
class ProcessControl;
using pPCB = std::shared_ptr<ProcessControl>;
using plist = std::list<pPCB>::iterator;
constexpr int RESOURCE = 5;

enum Priority {
	INIT = 0,
	USR = 1,
	SYS = 2,
};

class ResourceManger {
public:
	ResourceManger(int init = 0);
	int getAvailableNum();
	void allocate(int num);
	void recover(int num);
	void pushBackToWaiting(pPCB ptr);
private:
	int available;
	std::list<pPCB> WaitingList;
};

class ScheduleManger {
public:
	ScheduleManger();
	plist pushBack(pPCB ptr);
	void remove(pPCB ptr);
	bool request(pPCB ptr, const std::string & res, int num);
	void release(pPCB ptr, const std::string & res, int num);
	void release(pPCB ptr);
	pPCB schedule();
private:
	std::vector<std::list<pPCB>> readyList;
	std::unordered_map<std::string, ResourceManger> resource;
};

class ProcessControl {
public:
	Priority getPriority();
	ProcessControl(const std::string &id, Priority p);
	void getResource(const std::string & id, int num);
	int showResource(const std::string & id);
private:
	friend class Manager;
	std::unordered_map<std::string, int> ownResource;
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
	void request(const std::string & id, int num);
	void timeOut();
private:
	void schedule();
	pPCB runningProcess;
	ScheduleManger scheduler;
};