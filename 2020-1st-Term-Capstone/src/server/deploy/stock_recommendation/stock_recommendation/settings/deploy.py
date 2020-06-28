from .base import *

DATABASES = {
	'default': {
		'ENGINE': 'django.db.backends.postgresql_psycopg2',
		'NAME': 'stockrmd',
		'USER': 'dbuser',
		'PASSWORD': 'dbpw1234',
		'HOST': 'ec2-52-79-155-70.ap-northeast-2.compute.amazonaws.com', # 'capstone_db',
	'PORT': '5432',
	},
}

DEBUG = True #False
ALLOWED_HOSTS = ['*']
USE_TZ = False
CORS_ORIGIN_ALLOW_ALL = True
CORS_ALLOW_CREDENTIALS = True

