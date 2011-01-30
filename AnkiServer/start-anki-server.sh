#!/bin/bash

SOURCE=$HOME/prj/lingwo/bbcom/AnkiServer
WEBAPP=$HOME/webapps/anki_server

#LOG_FILE=$HOME/logs/user/anki_server/access_log.%Y%m%d
LOG_FILE=$WEBAPP/access.log
PID_FILE=$WEBAPP/paster.pid

#ROTATELOGS="/home/dsnopek/lingwo-phpstack/httpd-2.2.15/bin/rotatelogs $LOG_FILE 86400"
PASTER=$HOME/prj/lingwo/python-env/bin/paster
SERVE_ARGS="--daemon --log-file=$LOG_FILE --pid-file=$PID_FILE"
#SERVE_ARGS=""

# make sure it isn't already running
if [ -e $PID_FILE ]; then
	paster_pid=`cat $PID_FILE`
	if ps -p $paster_pid | grep -e $paster_pid > /dev/null 2>&1; then
		echo "Already running with PID $paster_pid"
		exit 0;
	else
		rm -f $PID_FILE
	fi
fi

# put the application eggs on the path
export PYTHONPATH="$PYTHONPATH:$SOURCE"

# execute!
exec $PASTER serve $SERVE_ARGS $WEBAPP/production.ini

