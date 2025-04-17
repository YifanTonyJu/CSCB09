#!/bin/bash
# This script is used to download a web-page from the internet and save it to a file.
# Usage: ./file_transfer.sh <file_url> [max_attempts]

MAX_ATTEMPTS_NUM=5
GREEN="\033[0;32m"
if [ $# -eq 0 ]
then
    echo "This program requires at least one argument indicating the URL to download."
    echo "Usage: $0 <file_url> [max_attempts]"
    exit 1
elif [ $# -gt  2 ]
then
    echo "This program requires at least one argument indicating the URL to download, and a maximum of 2."
    echo "Usage: $0 <file_url> [max_attempts]"
    exit 1
elif [ $# -eq 2 ]
then
    MAX_ATTEMPTS_NUM=$2
fi

ATTEMPTS_NUM=1
while [ $ATTEMPTS_NUM -le $MAX_ATTEMPTS_NUM ]
do
    echo -e "Attempt #${GREEN}${ATTEMPTS_NUM} to download $1"
    if curl "$1" -o "file_transfer--$ATTEMPTS_NUM"
    then
        echo "Download successful!"
        echo "File saved in file_transfered_$ATTEMPTS_NUM"
        break
    else
        echo "Download failed! ..."
        ATTEMPTS_NUM=$((ATTEMPTS_NUM + 1))
    fi
done
exit 0
# Important to know:
# 1. fi after if
# 2. done after while
# 3. exit 0 at the end of the script
# 4. exit 1 when end with error
# 5. echo -e to print colored text
# 6. GREEN="\033[0;32m"