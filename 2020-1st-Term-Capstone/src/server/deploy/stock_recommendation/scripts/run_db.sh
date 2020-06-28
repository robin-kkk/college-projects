#!/bin/sh
NAME="mydb"
IMAGE="postgres:10"
VOLUME1="-v `pwd`/db/:/var/lib/postgresql/data/"
VOLUME2="-v `pwd`/config/postgresql/initdb.d/:/docker-entrypoint-initdb.d/"
PORT="-p 5432:5432"
ENV="-e POSTGRES_PASSWORD=postgres"
OPTION="-d"

docker run $PORT $OPTION $VOLUME1 $VOLUME2 --name $NAME $ENV $IMAGE
docker logs -f $NAME
