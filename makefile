checker: main.cpp
	g++ main.cpp -std=c++11 -o checker
clean:
	rm -f checker
clean_sample:
	rm -f checker
	rm -f sample
	rm -f .out.txt
	rm -f .run.sh
	rm -f .tmp.txt