#!/bin/bash
#for req 16

#4 scenarios tested: all should fail for individual iteration

#scenario 1 starts: we should find mmap.c inside the disk image

#even if output file originally was same as mmap.c, -o sets it to empty at first 
#so we are on great shape to compare the two files
sync
output=$(./extfat -x mmap.c -i test.image -o newfile.image 2>&1)
echo $output
#default is success
test_result=0

#the diff command compares the two files and returns "" or nothing if they are same
compare=$(diff examples/mmap.c newfile.image)
if [[ "$compare" == "" ]]; then
    echo "Program *correctly extracts the file* when -x is used."
else
    echo "Program *fails to extract the file properly* when -x is used."
    test_result=1
fi

#scenario 2 starts: program should output that pineapple wasnt found inside test.image
sync
output=$(./extfat -x pineapple -i test.image -o newfile.image 2>&1)
echo $output
find='ERROR: File not found in image!'
#default is success
if [[ "$output" == *"$find"* ]]; then
    echo "Program *correctly says that extract file is not found* when it is not inside the disk image."
else
    echo "Program *fails to mention* that file was not found when disk image doesn't contain it."
    test_result=1
fi

#scenario 3 starts: program should mention that input file is required
output=$(./extfat -x pineapple -o abc.txt 2>&1)
echo $output
find='-i must be followed by test.image file type'
#default is success
if [[ "$output" == *"$find"* ]]; then
    echo "Program *correctly mentions that extract command needs input file* when it is not provided."
else
    echo "Program *fails to mention that extract command needs input file* when it is not provided."
    test_result=1
fi

#scenario 4 starts: program should mention that output file is required
output=$(./extfat -x pineapple -i test.image 2>&1)
echo $output
find='ERROR: Need to specify output file'
#default is success
if [[ "$output" == *"$find"* ]]; then
    echo "Program *correctly mentions that extract command needs output file* when it is not provided."
else
    echo "Program *fails to mention that extract command needs output file* when it is not provided."
    test_result=1
fi

exit $test_result