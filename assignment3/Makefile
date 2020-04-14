all: *.cpp
	clang++ -std=c++11  -o symexwins symexwins.cpp -ldeepstate 
	afl-clang++ -o symexwins_alf symexwins.cpp -ldeepstate_AFL
	clang++ -g -o symexwins_lf symexwins.cpp -fsanitize=fuzzer -ldeepstate_LF

