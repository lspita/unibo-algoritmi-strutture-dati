MAINFILE="0001114169"
COMPILE="gcc -std=c90 -Wall -Wpedantic src/${MAINFILE}.c -o ${MAINFILE}"
TEST_START=0
TEST_END=5

eval "${COMPILE}"

for i in $(seq $TEST_START $TEST_END); do
  echo "\n-----------------------\nTEST ${i}\n-----------------------"
  eval "./${MAINFILE} test${i}.in"
  read _
done