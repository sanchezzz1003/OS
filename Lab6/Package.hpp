#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <zmq.hpp>
#include <string>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "Worker.hpp"

class Package {
    public:
        enum {
            ERR,            CREATE,         REMOVE,
            PING,           REPLY,          TAKEPORT,
            REPAR,          EXEC,           TERM,
            KILLNPASS,

            COLL_UPDATE,    COLL_NEW_TASK,  COLL_RESPOND,
            COLL_EXIT,      COLL_RES,       COLL_GET_RESULT,
            COLL_REMOVE
        };

        int type;
        int id;
        int data;

        Package() {}
        Package(Worker& from) { recv(from); }
        Package(int type, int id, int data) : type(type), id(id), data(data) {}

        int send(Worker& to) {
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            auto start = clock();
            while (true) {
                if (to.Sock().send(typeMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(idMes,   zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(dataMes, zmq::send_flags::dontwait) )
                {
                    return 1;
                }
                if (clock() - start > (CLOCKS_PER_SEC)) return 0;
            }
        }

        void recv(Worker& from) {
            zmq::message_t typeMes;
            zmq::message_t idMes;
            zmq::message_t dataMes;
            from.Sock().recv(typeMes, zmq::recv_flags::none);
            from.Sock().recv(idMes, zmq::recv_flags::none);
            from.Sock().recv(dataMes, zmq::recv_flags::none);
            type = *((int*)(typeMes.data()));
            id = *((int*)(idMes.data()));
            data = *((int*)(dataMes.data()));
        }

        std::string recv_string(Worker& from) {
            zmq::message_t pack;
            from.Sock().recv(pack, zmq::recv_flags::dontwait);
            char * text = (char *)pack.data();
            std::string res(text);
            return res;
        }

        std::vector <int> recv_vector(Worker& from) {
            zmq::message_t pack;
            from.Sock().recv(pack, zmq::recv_flags::dontwait);
            int * data = (int *)pack.data();
            std::vector <int> res;
            for (size_t i = 1; i <= data[0]; i++)
                res.push_back(data[i]);
            return res;
        }

        int recvCheck(Worker& from) {
            zmq::message_t typeMes;
            zmq::message_t idMes;
            zmq::message_t dataMes;
            auto start = clock();
            while (true) {
                if (from.Sock().recv(typeMes, zmq::recv_flags::dontwait) &&
                    from.Sock().recv(idMes, zmq::recv_flags::dontwait) &&
                    from.Sock().recv(dataMes, zmq::recv_flags::dontwait) )
                {
                    type = *((int*)(typeMes.data()));
                    id = *((int*)(idMes.data()));
                    data = *((int*)(dataMes.data()));
                    return 1;
                }
                if (clock() - start > (CLOCKS_PER_SEC)) return 0;
            }
        }
        int send_exec(Worker& to, std::string text, std::string pattern) {
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            zmq::message_t textMes(text.data(), sizeof(text.size() + 1));
            zmq::message_t patternMes(pattern.data(), sizeof(pattern.size() + 1));
            auto start = clock();
            while (true) {
                if (to.Sock().send(typeMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(idMes,   zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(dataMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(textMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(patternMes, zmq::send_flags::dontwait))
                {
                    return 1;
                }
                if (clock() - start > (CLOCKS_PER_SEC)) return 0;
            }
        }
        int send_res(Worker& to, std::vector <int> res) {
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            zmq::message_t textMes(res.data(), sizeof(int) * res.size());
            auto start = clock();
            while (true) {
                if (to.Sock().send(typeMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(idMes,   zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(dataMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait) &&
                    to.Sock().send(textMes, zmq::send_flags::dontwait))
                {
                    return 1;
                }
                if (clock() - start > (CLOCKS_PER_SEC)) return 0;
            }
        }
};

#endif