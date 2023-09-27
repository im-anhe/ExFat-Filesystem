#!/bin/bash
#tests if help instructions are printed or not. Requirement 3 from spreadsheet

#executing the extfat files
output=$(./extfat -h)
echo $output
# test_result is success by default
test_result=0

#if output to stdout is a proper help instruction, the test is passed. Otherwise failed.
if [ "$output" = "*****HELP TEXT*****
-i : for file input
-o : for file output
-h : for help text
-c : denotes copy
-v : for verify (input file is required)
-d : to print out directory of files in the disk image
-D : to delete a file
-x : to extract file from the disk image" ]; then
    echo "Program *outputs proper help instructions* when -h is typed on command line."

else
    echo "Program *does not output correctly* when -h is typed on command line."
    test_result=1
fi

#exit the test result obtained
exit $test_result