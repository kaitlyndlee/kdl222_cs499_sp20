all: *.cpp
	clang++ -std=c++11  -o string_test str_util.cpp Tests.cpp -ldeepstate 
	clang++ -std=c++11  -o string_test_swarm str_util.cpp Tests.cpp -ldeepstate -DDEEPSTATE_PURE_SWARM
	afl-clang++ -o string_AFL str_util.cpp Tests.cpp -ldeepstate_AFL
	afl-clang++ -o string_AFL_swarm str_util.cpp Tests.cpp -ldeepstate_AFL -DDEEPSTATE_PURE_SWARM	
	clang++ -g -o string_LF str_util.cpp Tests.cpp -ldeepstate_LF -fsanitize=fuzzer
	/home/user/deps/honggfuzz/hfuzz_cc/hfuzz-clang++ -o string_H str_util.cpp Tests.cpp -ldeepstate
