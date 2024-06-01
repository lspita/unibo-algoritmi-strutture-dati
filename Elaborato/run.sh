#! /bin/bash

TESTS_PATH="test/"
RESULTS_PATH="results/"

MAINFILE="0001114169"
COMPILE="gcc -std=c90 -Wall -Wpedantic ${MAINFILE}.c -o ${MAINFILE}"

TEST_START=$1
TEST_END=$2

printf "TESTS: $TEST_START to $TEST_END\n"

eval "${COMPILE}"

for i in $(seq $TEST_START $TEST_END); do
  printf "\n-----------------------\nTEST ${i}\n-----------------------\n"
  eval "./${MAINFILE} ${TESTS_PATH}test${i}.in > ${RESULTS_PATH}res${i}.out"
done