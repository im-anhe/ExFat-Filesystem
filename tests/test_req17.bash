#!/bin/bash
#for req 17
#2 scenarios tested: all should fail for individual iteration


#scenario 1 starts: create_image.bash as well as grading_tests.bash and system_tests.bash have been
#updated to contain a certain number of files inside the disk image manually and those files have 
#been stored inside findx and tested to see whether or not they are deleted when -D is used
sync
output=$(./extfat -d 2>&1)
echo "original 1st time printed directory: ${output}"
find1='mmap.c'
find2='fread.c'
find3='extfat.c'
find4='extractFile.c'
find5='routines.c'

./extfat -D fread.c

output=$(./extfat -d)
echo "1st file deleted printed directory: ${output}"
#default is success
test_result=0

#if fread.c still exists, the program failed to delete it using -D
if [[ "$output" == *"$find2"* ]]; then
    echo "Program *failed to delete the file from the image file* when -D is used."
    test_result=1
else
    echo "Program *correctly deletes the file from the image file* when -D is used."
fi


#scenario 2: deleting another file and making sure again that -D does delete
#specified file from disk image
sync
output=$(./extfat -d 2>&1)
echo "original 2nd time printed directory: ${output}"

./extfat -D routines.c

output=$(./extfat -d)
echo "2nd file deleted printed directory: ${output}"

#if routines.c still exists, the program failed to delete it using -D
if [[ "$output" == *"$find5"* || "$output" == *"$find2"* ]]; then
    echo "Program *failed to delete the file from the image file* when -D is used."
    test_result=1
else
    echo "Program *correctly deletes the file from the image file* when -D is used."
fi

exit $test_result
