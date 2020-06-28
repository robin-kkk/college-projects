#!/bin/sh
sudo find . -path "*/migrations/*.py" -not -name "__init__.py" -delete && sudo find . -path "*/migrations/*.pyc"  -delete ;
rm -dR ../db
rm -dR .static_root
