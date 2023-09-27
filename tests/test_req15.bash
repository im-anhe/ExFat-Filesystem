#!/bin/bash
#for req 15

#comments not added too much because the sentences inside echo "..." explain what is going on.
#2 scenarios tested

#scenario 1 starts: create_image.bash as well as grading_tests.bash and system_tests.bash have been
#updated to contain a certain number of files inside the disk image and those files have 
#been stored inside findx and tested to see whether or not they are printed
sync
output=$(./extfat -d 2>&1)
echo $output
find1='mmap.c'
find2='fread.c'
find3='extfat.c'
find4='extractFile.c'
find5='routines.c'
#default is success
test_result=0

#if any file is not found at all, program is incorrect.
if [[ "$output" == *"$find1"* && "$output" == *"$find2"* && "$output" == *"$find3"* && "$output" == *"$find4"* && "$output" == *"$find5"* ]]; then
    echo "Program *correctly lists the directory* when -d is used on updated create_image.bash."
else
    echo "Program *incorrectly lists the directory* when -d is used on updated create_image.bash."
    test_result=1
fi


#scenario 2 starts: create_image.bash as well as grading_tests.bash and system_tests.bash have been
#updated to contain a certain number of files inside the disk image. But, inside findx there are 
#some files that should not be printed by program.
output=$(./extfat -d 2>&1)
echo $output
find1='hello'
find2='read_write.c'
find3='routines.h'
find4='clean_up.bash'
find5='extfat.h'
#default is success

#the program should not have printed any of the findx files
if [[ "$output" == *"$find1"* || "$output" == *"$find2"* || "$output" == *"$find3"* || "$output" == *"$find4"* || "$output" == *"$find5"* ]]; then
    echo "Program *incorrectly lists the directory and prints random words* when -d is used on updated create_image.bash."
    test_result=1
else
    echo "Program *correctly does not print random files or directory* when -d is used on updated create_image.bash."
fi

exit $test_result