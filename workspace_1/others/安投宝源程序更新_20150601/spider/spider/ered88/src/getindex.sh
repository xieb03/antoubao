#!/bin/bash

source ./conf.sh

>data/id.list
for ((i=1;i<2;i++))
do
    eval "${GETPAGE}" -O - "\"${INDEXPAGE}\"" --post-data="\"${POSTDATA}${i}${POSTDATA1}\"" \
        | awk '{s=$0; while (match(s, "bidding-[0-9]+")) {print substr(s, RSTART+8, RLENGTH-8); s=substr(s, RSTART+1);}}' | sort | uniq>>data/id.list
    eval "${GETPAGE}" -O - "\"${INDEXPAGE1}\"" --post-data="\"${POSTDATA2}${i}\"" \
        | awk '{s=$0; while (match(s, "bidding-[0-9]+")) {print substr(s, RSTART+8, RLENGTH-8); s=substr(s, RSTART+1);}}' | sort | uniq>>data/id.list
    
    sleep 1
done

exit 0

