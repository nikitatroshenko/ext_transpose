#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

for i in {4..16}
do
    for j in {4..16}
    do
        ./test_generator.out $i $j
        ./ext_transpose.out
            # && echo -e "[${GREEN}PASS${NC}]\tfor ${i}x${j}" \
        cmp output.bin output.expected.bin \
            || echo -e "[${RED}FAIL]\tfor ${i}x${j}"
    done
done