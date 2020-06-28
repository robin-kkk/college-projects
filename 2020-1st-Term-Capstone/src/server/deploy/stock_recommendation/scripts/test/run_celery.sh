#!/bin/sh
if [ -z $1 ] ; then
	echo "Usage: ./run_celery.sh <Project Name>"
	exit 0
fi

PROJECTNAME=$1
OPTION="--loglevel=INFO"
COMMAND="celery -A $PROJECTNAME worker $OPTION -B"


