G 		= g++
flags 	= -std=c++14 -o
dll 	= -lzmq -pthread

main 	: main.cpp Worker.hpp Package.hpp node 	; $(G) $(flags) main main.cpp $(dll)
node 	: work.cpp Worker.hpp Package.hpp 		; $(G) $(flags) node work.cpp $(dll)
clean 	: ; rm main node
log		: ; rm Logs/*.txt