#include <iostream>
#include <zmq.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "Worker.hpp"
#include "Package.hpp"
#include <thread>

enum {
    ERR, CREATE,
    REMOVE, EXEC,
    PING, EXIT, UPDATE
};

std::vector<Worker*> lists;
std::map<int, int> NodesAdr;
std::string comId;
std::map <std::string, int> command = {
    {"create", CREATE}, {"exec", EXEC},
    {"remove", REMOVE}, {"ping", PING},
    {"q", EXIT}, {"update", UPDATE}
};
std::string text, pattern;
Package to, from;
int id, parent, listID;

void run_collector(int ParentPort) {
    Worker main_node (Worker::PARENT, -1, ParentPort);
    Package mess_to, mess_from;
    std::vector <int> tasks, pivet;
    while (true) {
        mess_from.recv(main_node);
        switch (mess_from.type) {
            case Package::COLL_UPDATE:
                std::cout << "coll :: have " << tasks.size() << " tasks" << std::endl;
                for (auto t : tasks) {
                    std::cout << t << " ";
                }
                std::cout << "\n";
                begin:
                for (int task : tasks) {
                    listID = NodesAdr[task];
                    mess_to.type = Package::COLL_GET_RESULT;
                    mess_to.id = task;
                    mess_to.send(*lists[listID]);
                    std::cout << "coll :: send " << task << "\n";
                    if(!mess_from.recvCheck(*lists[listID])) {
                        std::cout << "task " << task << " did not finished\n";
                    } else {
                        pivet.clear();
                        pivet = mess_from.recv_vector(*lists[listID]);
                        std::cout << task << " ::";
                        for (size_t j = 0; j < pivet.size(); j++)
                            std::cout << " " << pivet[j];
                        std::cout << ";\n";
                        auto pos = std::find(tasks.begin(), tasks.end(), task);
                        tasks.erase(pos);
                        goto begin;
                    }
                }
                break;
            case Package::COLL_NEW_TASK: {
                auto res = std::find(tasks.begin(), tasks.end(), mess_from.id);
                if (res != tasks.end()) {
                    mess_to.data = 1;
                } else {
                    mess_to.data = 0;
                    tasks.push_back(mess_from.id);
                }
                std::cout << "coll :: added new task " << mess_from.id << std::endl;
                break;
            }
            case Package::COLL_EXIT:
                std::cout << "coll :: exit" << std::endl;
                return;
                break;
            case Package::COLL_REMOVE:
                if (tasks.size() != 0) {
                    mess_to.data = 1;
                } else {
                    mess_to.data = 0;
                }
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mess_to.type = Package::COLL_RESPOND;
        mess_to.send(main_node);
    }
}

int main(void) {
    int exit = 0;
    Worker collector (Worker::COLLECTOR, -10);
    std::thread collector_th (run_collector, collector.Port());
    NodesAdr[-1] = -1;
    while(!exit) {
        std::cin >> comId;
        switch (command[comId]) {
            case CREATE: {
                int id, parent;
                std::cin >> id >> parent;
                if (NodesAdr.find(parent) == NodesAdr.end()) {
                    std::cout << "Error: Parent not found\n";
                    break;
                }
                if (NodesAdr.find(id) != NodesAdr.end()) {
                    std::cout << "Error: Already exists\n";
                    break;
                }
                if (parent == -1) {
                    lists.push_back(new Worker(Worker::NEW, id));
                    pid_t added = lists[lists.size() - 1]->Pid();
                    NodesAdr[id] = lists.size() - 1;
                    std::cout << "Ok: " << added << '\n';
                } else {
                    listID = NodesAdr[parent];
                    to.type = Package::CREATE;
                    to.id = id;
                    to.data = parent;
                    to.send(*lists[listID]);
                    if(!from.recvCheck(*lists[listID])) {
                        std::cout << "Error: Parent is unavailable\n";
                        break;
                    }
                    if (from.type != Package::ERR) {
                        std::cout << "Ok: " << from.data << '\n';
                        NodesAdr[id] = NodesAdr[parent];
                    } else {
                        std::cout << "Error: in creation\n";
                    }
                }
                break;
            } case REMOVE: {
                std::cin >> id;
                if (NodesAdr.find(id) == NodesAdr.end() || id == -1) {
                    std::cout << "Error:id: Not found\n";
                    break;
                }
                to.type = Package::COLL_REMOVE;
                to.data = 0;
                to.id = -1;
                to.send(collector);
                from.recv(collector);

                if (from.data == 1) {
                    std::cout << "Error: No remove while job\n";
                    break;
                }

                listID = NodesAdr[id];
                if (lists[listID]->Id() == id){
                    to.type = Package::KILLNPASS;
                    Worker* tmp = new Worker();
                    tmp->TakePortSetId(0);
                    to.data = tmp->Port();
                    to.send(*lists[listID]);
                    if(!from.recvCheck(*lists[listID])) {
                        std::cout << "Error: Worker is unavailable\n";
                        to.type = Package::TERM;
                        to.send(*tmp);
                        delete tmp;
                        break;
                    }
                    if (from.type != Package::ERR) {
                        tmp->Id() = from.id;
                        std::swap(tmp, lists[listID]);
                        delete tmp;
                        if (lists[listID]->Id() == -2) {
                            delete lists[listID];
                            lists[listID] = nullptr;
                        }
                        std::cout << "Ok\n";
                        NodesAdr.erase(id);
                    } else {
                        std::cout << "Error: Worker is unavailable\n";
                        to.type = Package::TERM;
                        to.send(*tmp);
                        delete tmp;
                    }
                    break;
                } else {
                    to.type = Package::REMOVE;
                    to.id = id;
                    to.send(*lists[listID]);
                    if(!from.recvCheck(*lists[listID])) {
                        std::cout << "Error: Worker is unavailable\n";
                        break;
                    }
                    if (from.type != Package::ERR) {
                        std::cout << "Ok\n";
                        NodesAdr.erase(id);
                    } else {
                        std::cout << "Error: Worker is unavailable\n";
                    }
                }
                break;
            } case UPDATE: {
                to.type = Package::COLL_UPDATE;
                to.data = 0;
                to.id = -1;
                to.send(collector);
                from.recv(collector);
                break;
            } case EXEC: {
                std::cin >> id;
                if (NodesAdr.find(id) == NodesAdr.end()) {
                    std::cout << "Error:id: Not found\n";
                    break;
                }
                listID = NodesAdr[id];

                to.type = Package::COLL_NEW_TASK;
                to.id = id;
                to.send(collector);

                if (!from.recvCheck(collector)) {
                    std::cout << "Error: create task" << std::endl;
                    break;
                };

                if (from.data == 0) {
                    to.type = Package::EXEC;
                    to.id = id;
                    std::cin >> text >> pattern;
                    to.send_exec(*lists[listID], text, pattern);
                } else {
                    std::cout << "node is busy" << std::endl;
                }
                break;
            } case PING: {
                std::cin >> id;
                if (NodesAdr.find(id) == NodesAdr.end() || id == -1) {
                    std::cout << "Error: Not found\n";
                    break;
                }
                listID = NodesAdr[id];
                to.type = Package::PING;
                to.id = id;
                to.send(*lists[listID]);
                if(!from.recvCheck(*lists[listID])) {
                    std::cout << "Ok: 0\n";
                } else {
                    std::cout << "Ok: 1\n";
                }
                break;
            } case EXIT: {
                exit = 1;
                to.type = Package::COLL_EXIT;
                to.send(collector);
                collector_th.join();
                std::cout << "out\n";
                break;
            } case ERR: {
                std::cout << "Wrong Command\n";
                break;
            }
        }
    }
    for (int i = 0; i < lists.size(); i++) {
        to.type = Package::TERM;
        if (lists[i] != nullptr) {
            to.send(*lists[i]);
            std::cout << "send\n";
        }
        delete lists[i];
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}