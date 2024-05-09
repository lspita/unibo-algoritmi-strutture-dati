#!/bin/bash

SRC_PATH="src/"
TESTS_PATH="test/"

MAINFILE="0001114169"
COMPILE="gcc -std=c90 -Wall -Wpedantic ${SRC_PATH}/${MAINFILE}.c -o ${MAINFILE}"

TEST_START=$1
TEST_END=$2

printf "TESTS: $TEST_START to $TEST_END\n\n"

eval "${COMPILE}"

for i in $(seq $TEST_START $TEST_END); do
  printf "\n-----------------------\nTEST ${i}\n-----------------------\n"
  eval "./${MAINFILE} ${TESTS_PATH}test${i}.in"
  read _
done