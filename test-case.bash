#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
NC='\033[0m'

TEST_FILES_DIR="./tests/$1.$2"
ER_FILE_NAME="output.expected.bin"
INPUT_FILE_NAME="input.bin"
INPUT=$TEST_FILES_DIR/$INPUT_FILE_NAME
ER=$TEST_FILES_DIR/$ER_FILE_NAME
AR="output.bin"

if [[ -d $TEST_FILES_DIR ]]
then
    cp $ER ./$ER_FILE_NAME
    cp $INPUT ./$INPUT_FILE_NAME
else
    ./test_generator.out $1 $2
    mkdir -p $TEST_FILES_DIR
    cp ./$ER_FILE_NAME $ER
    cp ./$INPUT_FILE_NAME $INPUT
fi

START_TIME="$(date -u +%s.%N)"
./ext_transpose.out
END_TIME="$(date -u +%s.%N)"
cmp $ER_FILE_NAME $AR
RC=$?
ELAPSED="$(bc <<<"$END_TIME-$START_TIME")"
if [[ $RC -ne 0 ]]
then
    echo -e "[${RED} WA  ${NC}]\tfor ${1}x${2}"
elif [[ $(bc <<<"$ELAPSED>1") -eq 1 ]]
then
    echo -e "[${ORANGE} TLE ${NC}]\t ${ELAPSED}s for ${1}x${2}"
else
    echo -e "[${GREEN} OK  ${NC}]\tfor ${1}x${2}"
fi