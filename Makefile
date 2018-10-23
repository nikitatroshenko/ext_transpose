TRANSPOSE_BLOCK_SIZE=512
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
AR_OUTPUT='output.bin'
ER_OUTPUT='output.expected.bin'

all: executeables

executeables: test_generator.out ext_transpose.out

test_generator.out:
	g++ -o test_generator.out test_generator.cpp

ext_transpose.out:
	g++ -o ext_transpose.out -DDEFAULT_BLOCK_SIZE=$(TRANSPOSE_BLOCK_SIZE) main.cpp

clean:
	rm -f *.out *.bin