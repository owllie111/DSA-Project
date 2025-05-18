#!/usr/bin/env bash

# Change this line to your tail command from GNU (that accepts tail -n +linenumber) 
#TAIL=/opt/homebrew/bin/gtail  # for MacOS with brew install coreutils
TAIL=/usr/bin/tail           # on Linux distribution


# Change this to point to your sched executable or you will have to pass it
# as 1st parameter on the command line
EXEC=../src/sched 
if [ $# -eq 1 ]; then
    EXEC=$1    
fi

PATTERN="===Results==="
DATA_IN="in"
DATA_REF="ref"
DATA_OUT="out"
mkdir -p $DATA_OUT

echo "* Using executable ${EXEC}"


COMMENT_CHAR="#"
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

IN_EXT=".in"

if [ $# -eq 2 ]; then  # allow to specify some tests only as param2
    test_files=$2
else
    test_files=$DATA_IN/*$IN_EXT
fi
echo "* Testing on: " $test_files

for test_in in $test_files
do
    t=$(basename $test_in $IN_EXT)
    printf "* Testing $t ..."
    input="$DATA_IN/$t$IN_EXT"
    output="$DATA_OUT/$t.out"
    output_res="$DATA_OUT/$t.res"
    refs="$DATA_REF/$t*.ref" 
    $EXEC < $input > $output
    nl=$(grep -n $PATTERN < $output | cut -f1 -d':')
    if [ -z "$nl" ]; then
        printf "   [${RED}*Error: could not fid patern \"$PATTERN\" in $output${NC}]\n"
        exit 1
    fi
    $TAIL -n +$nl < $output > $output_res

    # --- now tests against all possible references results
    NOK=True
    for ref in $refs
    do
        res_diff=$(diff $ref $output_res)
        if [ -z "$res_diff" ]; then
            NOK=False
            match=$ref
        fi
     done
     if [ "$NOK" == "True" ]; then 
         printf "${RED}NOK${NC} (could not match any ref)\n"
        res_diff=$(diff -y $ref $output_res)
        printf "* diff: <reference (left)>  <you (right)> \n$res_diff${NC}\n\n"
    else
        printf " ${GREEN}OK${NC} (matches $match)\n"
    fi
done
exit 0
