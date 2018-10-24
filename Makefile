TRANSPOSE_BLOCK_SIZE=512
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
AR_OUTPUT='output.bin'
ER_OUTPUT='output.expected.bin'

all: executeables test

test: executeables
	echo "Running tests..."
	./test-case.bash 512 512
	./test-case.bash 1024 1024
	./test-case.bash 2048 1024
	./test-case.bash 1024 2048
	./test-case.bash 1000 800
	./test-case.bash 3000 3000
	./test-case.bash 300 200
	./test-case.bash 500 20000
	./test-case.bash 1 10000000
	./test-case.bash 10000000 1
	./test-case.bash 2 5000000
	./test-case.bash 5000000 2
	./test-case.bash 100 100

executeables: test_generator.out ext_transpose.out

test_generator.out:
	g++ -o test_generator.out test_generator.cpp

ext_transpose.out:
	g++ --std=c++11 -o ext_transpose.out -DDEFAULT_BLOCK_SIZE=$(TRANSPOSE_BLOCK_SIZE) main.cpp

clean:
	rm -f *.out

clean_tests:
	rm -f *.bin

clean_all: clean clean_tests