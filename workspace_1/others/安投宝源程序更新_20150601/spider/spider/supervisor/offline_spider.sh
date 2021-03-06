#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: \$0 site1 [site2] [...]"
    exit 0
fi

export PATH="${PATH}:/usr/local/services/store/"

mkdir -p /data/log/supervisor/
LOGFILE="/data/log/supervisor/offline_spider.log"

cd /usr/local/services/offline_spider

for SITE in $@
do
    if [ -d ${SITE} ]
    then
        cd ${SITE}
        if [ -f "spider.running" ]
        then
            echo "`/bin/date "+%Y-%m-%d %H:%M:%S"`, ${SITE} is still running, skipped" >>${LOGFILE}
        else
            if [ ! -d "html" ]
            then
                mkdir -p /data/spider/${SITE}/html
                ln -s /data/spider/${SITE}/html html
            fi

            if [ ! -d "data" ]
            then
                mkdir -p /data/spider/${SITE}/data
                ln -s /data/spider/${SITE}/data data
            fi

            if [ ! -f "error.log" ]
            then
                mkdir -p /data/log/spider/
                touch /data/log/spider/${SITE}.log
                ln -s /data/log/spider/${SITE}.log error.log
            fi

            bash spider_main.sh &

            echo "`/bin/date "+%Y-%m-%d %H:%M:%S"`, ${SITE} started" >>${LOGFILE}
        fi
        cd -
    fi
done

exit 0

