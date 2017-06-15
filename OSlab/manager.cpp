#include "manager.h"
using namespace std;

Manager::Manager() {
	runningProcess = make_shared<ProcessControl>("init", INIT);
	listManager.pushBack(runningProcess);
	schedule();
}

std::string Manager::getCurPID() {
	return runningProcess->pid;
}

void Manager::createProcess(const std::string & id, Priority p) {
	auto newProcess = make_shared<ProcessControl>(id, p);
	newProcess->parent = runningProcess;
	runningProcess->children.push_back(newProcess);
	listManager.pushBack(newProcess);
	schedule();
}

void Manager::requestResource(const std::string & id, int num) {
	if (!listManager.requestResource(runningProcess, id, num))
		schedule();
}

void Manager::releaseResource(const std::string & id, int num) {
}

void Manager::destroy(const std::string & id) {
	destroy(listManager.getPCBbyName(id));
}

void Manager::timeOut() {
	listManager.removeFromReadyList(runningProcess);
	listManager.pushBack(runningProcess);
	schedule();
}

void Manager::destroy(pPCB p) {
	if (p == nullptr) return;
	for (auto child : p->children) destroy(child);
	listManager.releaseResource(p);
	listManager.remove(p);
	schedule();
}

void Manager::schedule() {
	runningProcess = listManager.schedule();
}

ListManger::ListManger() :readyList(SYS + 1) {
	for (int i = 1; i < RESOURCE; i++)
		resource.emplace("R" + to_string(i), ResourceManger("R" + to_string(i), i));
}

plist ListManger::pushBack(pPCB pointer) {
	auto &ref = readyList[pointer->getPriority()];
	return ref.insert(ref.end(), pointer);
}

void ListManger::removeFromReadyList(pPCB ptr) {
	readyList[ptr->getPriority()].remove(ptr);
}

void ListManger::removeFromWaitingList(pPCB ptr) {
	for (auto &l : resource)
		l.second.remove(ptr);
}

void ListManger::remove(pPCB ptr) {
	removeFromReadyList(ptr);
	removeFromWaitingList(ptr);
}

bool ListManger::requestResource(pPCB ptr, const std::string & res, int num) {
	ptr->getResource(res, num);
	if (resource[res].getAvailableNum() >= num) {
		resource[res].allocate(num);
		return true;
	}
	else {
		removeFromReadyList(ptr);
		resource[res].pushBackToWaiting(ptr);
		return false;
	}
}

void ListManger::releaseResource(pPCB ptr, const std::string & res, int num) {
	auto l = resource[res].recover(ptr->showResource(res));
	for (auto p : l) readyList[p->getPriority()].push_back(p);
}

void ListManger::releaseResource(pPCB ptr) {
	for (auto& p : resource) {
		auto l = p.second.recover(ptr->showResource(p.first));
		for (auto p : l) readyList[p->getPriority()].push_back(p);
	}
}

pPCB ListManger::schedule() {
	if (!readyList[SYS].empty()) return readyList[SYS].front();
	if (!readyList[USR].empty()) return readyList[USR].front();
	return readyList[INIT].front();
}

pPCB ListManger::getPCBbyName(const std::string & str) {
	for (const auto &l : readyList) {
		for (const auto &p : l) if (p->getPID() == str) return p;
	}
	for (const auto &r : resource) {
		auto res = r.second.getPCBbyName(str);
		if (res) return res;
	}
	return nullptr;
}

ProcessControl::ProcessControl(const std::string & id, Priority p) : pid(id), priority(p), ownResource(RESOURCE) {
}

void ProcessControl::getResource(const std::string & id, int num) {
	ownResource[id] += num;
}

int ProcessControl::showResource(const std::string & id) {
	return ownResource[id];
}

std::string ProcessControl::getPID() {
	return pid;
}

Priority ProcessControl::getPriority() {
	return priority;
}

ResourceManger::ResourceManger(const std::string & str, int init) :name(str), available(init) {
}

int ResourceManger::getAvailableNum() {
	return available;
}

void ResourceManger::allocate(int num) {
	available -= num;
}

std::list<pPCB> ResourceManger::recover(int num) {
	available += num;
	std::list<pPCB> ready;
	for (auto ite = waitingList.begin();
		ite != waitingList.end() && (*ite)->showResource(name) <= available;
		ite = waitingList.erase(ite)) {
		ready.push_back(*ite);
		available -= (*ite)->showResource(name);
	}
	return ready;
}

void ResourceManger::pushBackToWaiting(pPCB ptr) {
	waitingList.push_back(ptr);
}

void ResourceManger::remove(pPCB ptr) {
	waitingList.remove(ptr);
}

pPCB ResourceManger::getPCBbyName(const std::string & str) const {
	for (auto p : waitingList)
		if (p->getPID() == str)
			return p;
	return nullptr;
}
