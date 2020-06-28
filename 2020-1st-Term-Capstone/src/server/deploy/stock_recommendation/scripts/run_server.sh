#!/bin/sh

if [ -z $1 ] ; then
	echo "Usage: ./run_server.sh <Project Name>"
	exit 0
fi

PROJECT_NAME=$1

gunicorn --chdir /code \
	--env DJANGO_SETTINGS_MODULE=$PROJECT_NAME.settings.deploy \
	--bind :8000 $PROJECT_NAME.wsgi:application

