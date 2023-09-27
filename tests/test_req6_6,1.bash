#!/bin/bash
#for req 6 and 6.1

#2 scenarios tested

#scenario 1 starts: regular test.image means program should say both MBR and BBR match
sync
output=$(./extfat -i test.image -v 2>&1)
echo $output
find='Both main and boot sectors match!'
#default is success
test_result=0

#if a success indicating sentence is found, program was correct
if [[ "$output" == *"$find"* ]]; then
    echo "Program *correctly says MBR and BBR are same* when MBR and BBR are the same."
else
    echo "Program *incorrectly says MBR and BBR are different* when MBR and BBR are the same."
    test_result=1
fi



#scenario 2 starts: 1 byte modified test.image means that program should say they differ
gcc -o tests/write_extfat tests/write_extfat.c
./tests/write_extfat

sync
output=$(./extfat -i test.image -v 2>&1)
echo $output
find='Main and backup boot sectors do not match!'

#if a failure indicating sentence is found, program was correct. We are supposed to find issue here.
if [[ "$output" == *"$find"* ]]; then
    echo "Program *correctly says MBR and BBR are different* when MBR and BBR are altered and not same."
else
    echo "Program *incorrectly says MBR and BBR are same* when MBR and BBR are altered and not same."
    test_result=1
fi

exit $test_result