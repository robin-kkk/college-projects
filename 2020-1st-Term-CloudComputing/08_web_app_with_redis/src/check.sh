#!/bin/sh
NAME=$1
docker exec -it $NAME python -c "from utils import *; r=RedisSession(); print('generated key #', len(r.keys()));"
