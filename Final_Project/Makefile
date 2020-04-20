all: *.cpp
	clang++ -std=c++11  -o string_test str_util.cpp Tests.cpp -ldeepstate 
	afl-clang++ -o string_AFL str_util.cpp Tests.cpp -ldeepstate_AFL
	clang++ -g -o string_LF str_util.cpp Tests.cpp -ldeepstate_LF -fsanitize=fuzzer

