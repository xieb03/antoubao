#!/bin/bash

source ./conf.sh

# get index page
>data/id.list.tmp
for((i=1;i<=4;i++))
do
    eval "${GETPAGE}" -O - "\"${INDEXPAGE}${i}\"" \
        | awk '{s=$0; while (match(s, "/invest/[A-Z]+[0-9]+\.html")) {print substr(s, RSTART+8, RLENGTH-13); \
		s=substr(s, RSTART+1);}}' | sort | uniq>>data/id.list.tmp
    sleep 1
done

touch data/id.list.old
sort data/id.list.tmp data/id.list.old | uniq >data/id.list
mv data/id.list.tmp data/id.list.old

exit 0

