#include <zmq.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <thread>
#include "Worker.hpp"
#include "Package.hpp"

enum {
    WORKER_EMPTY,
    WORKER_FINISHED,
    WORKER_WORKING
};
std::mutex W_lock;
std::vector <int> result_of_work;
int worker_state = WORKER_EMPTY;
std::thread * worker = NULL;

std::vector <int> findPatt(std::string text, std::string patern) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << text << " " << patern << "\n";
    std::vector <int> pos;	bool par;
    for (size_t i = 0; i < static_cast<size_t>(text.size()); i++) {
        par = true;
        if (text.size() - i < patern.size())
            break;
        for (size_t j = 0; j < patern.size(); j++)
            if (patern[j] != text[i + j]) {
                par = false;
                break;
            }
        if (par)
            pos.push_back(i);
    }
    for (int a : pos) {
        std::cout << a << " ";
    }
    std::cout << "\n";
    if (pos.size() == 0) {
        pos.push_back(1);
        pos.push_back(-1);
    } else {
        pos.insert(pos.begin(), pos.size());
    }
    return pos;
}

void foo(std::string text, std::string patt) {
    std::vector <int> res = findPatt(text, patt);
    W_lock.lock();
    worker_state = WORKER_FINISHED;
    result_of_work.clear();
    result_of_work = res;
    W_lock.unlock();
    return;
}

int main(int argc, char ** argv) {
	if (argc != 2)
		return 0;
    int Id = atoi(argv[0]);
	Worker * Parent = new Worker(Worker::PARENT, 0, atoi(argv[1]));
    Worker * Child = new Worker;
    Worker * tmp;
    Package to, from;

    while(true) {
        from.recv(*Parent);
        switch (from.type) {
            case Package::CREATE: {
                if (Id == from.data) {
                    if (Child->Id() == -2) {
                        Child->New(from.id);
                        from.type = Package::REPLY;
                        from.data = Child->Pid();
                        from.send(*Parent);
                    } else {
                    	from.type = Package::ERR;
						from.send(*Parent);
                    }
                } else {
                    from.send(*Child);
                    if (!to.recvCheck(*Child)) break;
                    else to.send(*Parent);
                }
                break;
			} case Package::REMOVE: {
                if (Child->Id() == from.id){
                    tmp = new Worker();
                    tmp->TakePortSetId(0);
                    to.type = Package::KILLNPASS;
                    to.data = tmp->Port();
                    to.send(*Child);
                    if(!to.recvCheck(*Child)) {
                        to.type = Package::TERM;
                        to.send(*tmp);
                        delete tmp;
                        break;
                    }
                    if (to.type != Package::ERR) {
                        tmp->Id() = to.id;
                        std::swap(tmp, Child);
                        delete tmp;
                    } else {
                        std::cout << "Error: while removing\n";
                    }
                    from.send(*Parent);
                    break;
                } else {
                    from.send(*Child);
                    if(!to.recvCheck(*Child)) { break; }
					else { to.send(*Parent); }
                    break;
                }
			} case Package::KILLNPASS: {
                if (Child->Id() != -2) {
                    to.type = Package::REPAR;
                    to.data = from.data;
                    to.send(*Child);
                }
                to.type = Package::REPLY;
                to.id = Child->Id();
                to.send(*Parent);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return 0;
                break;
			} case Package::TAKEPORT: {
                Child->TakePortSetId(from.id);
                from.type = Package::REPLY;
                from.data = Child->Port();
                from.send(*Parent);
                break;
			} case Package::REPAR: {
                tmp = new Worker(Worker::PARENT, 0, from.data);
                std::swap(tmp, Parent);
                delete tmp;
                break;
			} case Package::TERM: {
                if (Child->Id() != -2)
                    from.send(*Child);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return 0;
                break;
			} case Package::EXEC: {
                if (Id == from.id && worker_state == WORKER_EMPTY) {
                    worker_state = WORKER_WORKING;
                    std::string text = from.recv_string(*Parent);
                    std::string patt = from.recv_string(*Parent);
                    worker = new std::thread(foo, text, patt);
                } else if (Child->Id() != -2) {
                    std::string text = from.recv_string(*Parent);
                    std::string patt = from.recv_string(*Parent);
                    from.send_exec(*Child, text, patt);
                }
                break;
			} case Package::COLL_GET_RESULT: {
                if (from.id == Id) {

                    W_lock.lock();
                    if (worker_state == WORKER_FINISHED) {
                        to.type = Package::COLL_RES;
                        to.id = -10;
                        to.data = Id;
                        to.send_res(*Parent, result_of_work);
                        worker_state = WORKER_EMPTY;
                        worker->join();
                        // delete worker;
                    }
                    W_lock.unlock();

                } else if (Child->Id() != -2) {
                    from.send(*Child);
                    if (!to.recvCheck(*Child))
                        break;
                    else
                        to.send_res(*Parent, to.recv_vector(*Child));
                }
                break;
			} case Package::PING: {
                if (Id == from.id) {
                    to.type = Package::REPLY;
                    to.send(*Parent);
                } else {
                    from.send(*Child);
                    if (!to.recvCheck(*Child))
                        break;
                    else
                        to.send(*Parent);
                }
                break;
			}
        }
    }
	return 0;
}