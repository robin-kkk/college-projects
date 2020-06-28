#!/bin/sh

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] || [ -z $4 ] ; then
	echo "Usage: ./init.sh <PROJECT_NAME> <ADMIN_NAME> <ADMIN_PASSWORD> <ADMIN_MAIL>"
	exit 0
fi

PROJECT_NAME=$1
ADMIN_NAME=$2
ADMIN_PASSWORD=$3
ADMIN_MAIL=$4

python manage.py collectstatic --no-input && \
python manage.py makemigrations && ./manage.py migrate && \
echo "from django.contrib.auth.models import User; User.objects.create_superuser('$ADMIN_NAME', '$ADMIN_MAIL', '$ADMIN_PASSWORD')" | python manage.py shell && \
echo "from item.utils import generate; generate();" | python manage.py shell && \
./scripts/run_server.sh $PROJECT_NAME
#./manage.py runserver

