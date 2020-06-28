#!/bin/sh
NAME="mydb"
USERNAME="dbuser"
DBNAME="stockrmd"

docker exec -it $NAME psql -U $USERNAME -d $DBNAME
