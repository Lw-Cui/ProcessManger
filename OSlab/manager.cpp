#include "manager.h"
using namespace std;

Manager::Manager() {
	runningProcess = make_shared<ProcessControl>("init", INIT);
	scheduler.pushBack(runningProcess);
	schedule();
}

std::string Manager::getCurPID() {
	return runningProcess->pid;
}

void Manager::createProcess(const std::string & id, Priority p) {
	auto newProcess = make_shared<ProcessControl>(id, p);
	newProcess->parent = runningProcess;
	runningProcess->children.push_back(newProcess);
	scheduler.pushBack(newProcess);
	schedule();
}

void Manager::request(const std::string & id, int num) {
	if (!scheduler.request(runningProcess, id, num))
		schedule();
}

void Manager::timeOut() {
	scheduler.remove(runningProcess);
	scheduler.pushBack(runningProcess);
	schedule();
}

void Manager::schedule() {
	runningProcess = scheduler.schedule();
}

ScheduleManger::ScheduleManger() :readyList(SYS + 1) {
	for (int i = 1; i < RESOURCE; i++)
		resource.emplace("R" + to_string(i), i);
}

plist ScheduleManger::pushBack(pPCB pointer) {
	auto &ref = readyList[pointer->getPriority()];
	return ref.insert(ref.end(), pointer);
}

void ScheduleManger::remove(pPCB ptr) {
	readyList[ptr->getPriority()].remove(ptr);
}

bool ScheduleManger::request(pPCB ptr, const std::string & res, int num) {
	if (resource[res].getAvailableNum() >= num) {
		ptr->getResource(res, num);
		resource[res].allocate(num);
		return true;
	}
	else {
		remove(ptr);
		resource[res].pushBackToWaiting(ptr);
		return false;
	}
}

void ScheduleManger::release(pPCB ptr, const std::string & res, int num) {
	resource[res].recover(ptr->showResource(res));
}

void ScheduleManger::release(pPCB ptr) {
	for (auto& p : resource) {
		p.second.recover(ptr->showResource(p.first));
	}
}

pPCB ScheduleManger::schedule() {
	if (!readyList[SYS].empty()) return readyList[SYS].front();
	if (!readyList[USR].empty()) return readyList[USR].front();
	return readyList[INIT].front();
}

ProcessControl::ProcessControl(const std::string & id, Priority p) : pid(id), priority(p), ownResource(RESOURCE) {
}

void ProcessControl::getResource(const std::string & id, int num) {
	ownResource[id] += num;
}

int ProcessControl::showResource(const std::string & id) {
	return ownResource[id];
}

Priority ProcessControl::getPriority() {
	return priority;
}

ResourceManger::ResourceManger(int init) :available(init) {
}

int ResourceManger::getAvailableNum() {
	return available;
}

void ResourceManger::allocate(int num) {
	available -= num;
}

void ResourceManger::recover(int num) {
	available += num;
}

void ResourceManger::pushBackToWaiting(pPCB ptr) {
	WaitingList.push_back(ptr);
}
