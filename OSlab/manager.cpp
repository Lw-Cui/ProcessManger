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
		resouce.emplace_back(i);
}

plist ScheduleManger::pushBack(pPCB pointer) {
	auto &ref = readyList[pointer->getPriority()];
	return ref.insert(ref.end(), pointer);
}

void ScheduleManger::remove(pPCB ptr) {
	readyList[ptr->getPriority()].remove(ptr);
}

void ScheduleManger::request(pPCB ptr, int res, int num) {
	if (resouce[res].getAvailableNum() >= num) {
			
	}
	else {
	}
}

void ScheduleManger::release(pPCB ptr, int res, int num) {
}

pPCB ScheduleManger::schedule() {
	if (!readyList[SYS].empty()) return readyList[SYS].front();
	if (!readyList[USR].empty()) return readyList[USR].front();
	return readyList[INIT].front();
}

ProcessControl::ProcessControl(const std::string & id, Priority p) : pid(id), priority(p) {
}

Priority ProcessControl::getPriority() {
	return priority;
}

ResourceManger::ResourceManger(int init) :initNum(init) {
}

int ResourceManger::getAvailableNum() {
	return available;
}

void ResourceManger::pushBackToWaiting(pPCB ptr) {
	WaitingList.push_back(ptr);
}
