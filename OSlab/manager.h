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
	ResourceManger(const std::string &str = "", int init = 0);
	int getAvailableNum();
	void allocate(int num);
	std::list<pPCB> recover(int num);
	void pushBackToWaiting(pPCB ptr);
	void remove(pPCB ptr);
	pPCB getPCBbyName(const std::string &str) const;
private:
	std::string name;
	int available;
	std::list<pPCB> waitingList;
};

class ListManger {
public:
	ListManger();
	plist pushBack(pPCB ptr);
	void removeFromReadyList(pPCB ptr);
	void removeFromWaitingList(pPCB ptr);
	void remove(pPCB ptr);
	bool requestResource(pPCB ptr, const std::string & res, int num);
	void releaseResource(pPCB ptr, const std::string & res, int num);
	void releaseResource(pPCB ptr);
	pPCB schedule();
	pPCB getPCBbyName(const std::string &str);
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
	std::string getPID();
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
	void requestResource(const std::string & id, int num);
	void releaseResource(const std::string & id, int num);
	void destroy(const std::string & id);
	void timeOut();
private:
	void destroy(pPCB);
	void schedule();
	pPCB runningProcess;
	ListManger listManager;
};