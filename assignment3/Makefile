all: *.cpp
	clang++ -std=c++11  -o symexwins symexwins.cpp -ldeepstate 
	afl-clang++ -o symexwins_afl symexwins.cpp -ldeepstate_AFL
	clang++ -g -o symexwins_lf symexwins.cpp -fsanitize=fuzzer -ldeepstate_LF

	afl-clang -o fuzzingwins fuzzingwins.cpp -ldeepstate 
	afl-clang++ -o fuzzingwins_afl fuzzingwins.cpp -ldeepstate_AFL
	clang++ -g -o fuzzingwins_lf fuzzingwins.cpp -fsanitize=fuzzer -ldeepstate_LF

