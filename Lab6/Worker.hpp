#ifndef WORKER_HPP
#define WORKER_HPP

#include <zmq.hpp>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <thread>

int BEGIN_PORT = 10000;
const char * BIND_URL = "tcp://*:";
const char * CON_URL = "tcp://localhost:";

std::string BindURLPort(int port) { return BIND_URL + std::to_string(port); }
std::string ConURLPort(int port) { return CON_URL + std::to_string(port); }

int TakePort(zmq::socket_t& sock) {
	int port = BEGIN_PORT;
	while (true) {
		try { sock.bind(BindURLPort(port));
			  break;
		} catch (const zmq::error_t& err) { ++port; }
	}
	return port;
}

class Worker {
    public:
        enum {
            ERR, CHILD,
            PARENT, NEW, ADD, COLLECTOR
        };
        Worker() : id(-2), ctx(1), sock(ctx, ZMQ_PAIR) {}
        Worker(int type, int id, int inPort = -1) : ctx(1), sock(ctx, ZMQ_PAIR), id(id) {
            switch(type) {
                case CHILD:
                    port = inPort;
                    sock.bind(BindURLPort(port));
                    break;
                case PARENT:
                    port = inPort;
                    sock.connect(ConURLPort(port));
                    break;
                case NEW:
                    port = TakePort(sock);
                    pid = fork();
                    if (pid == 0) {
                        using std::to_string;
                        execl("node", to_string(id).c_str(), to_string(port).c_str(), (char*)NULL);
                    }
                    break;
				case COLLECTOR:
					port = TakePort(sock);
					break;

            }
        }
        void New(int inId) {
            id = inId;
            port = TakePort(sock);
            pid = fork();
            if (pid == 0) {
                using std::to_string;
                execl("node", to_string(id).c_str(), to_string(port).c_str(), (char*)NULL);
            }
        }
        void TakePortSetId(int Id) {
            id = Id;
            port = TakePort(sock);
        }
        void disConnect() { sock.disconnect(ConURLPort(port)); }
        pid_t Pid() 			{ return pid; }
        int& Id() 				{ return id;  }
        int Port() 				{ return port;}
        zmq::socket_t& Sock() 	{ return sock;}

    private:
        int id;
        int port;
        pid_t pid;
        zmq::context_t ctx;
        zmq::socket_t sock;
};

#endif