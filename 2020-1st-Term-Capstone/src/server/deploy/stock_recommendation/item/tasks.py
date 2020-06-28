#-*- coding:utf-8 -*-
from __future__ import absolute_import
from celery.schedules import crontab
from django.db import transaction
from stock_recommendation.celery import app
from item.models import StockItem, StockNews
from item.utils import *

@app.on_after_finalize.connect
def setup_periodic_tasks(sender, **kwargs):
	sender.add_periodic_task(
		crontab(minute='*/10',hour='7,8,9,10,11,12,13,14,15,16,17,18'),
		update_stock_data.s()
	)

	sender.add_periodic_task(
		crontab(minute=0, hour='*/1,9-19'),
		update_news_data.s()
	)


@app.task
def update_stock_data():
	items = StockItem.objects.all()
	for item in items:
		with transaction.atomic():
			try:
				d = get_changing_data(item.code)
				item.now_val = d['now_val']
				item.fluctuation = d['fluctuation']
				item.diff = d['diff']
				item.volume = d['volume']
				item.save()
			except Exception as e:
				print("[-] update_stock_data(): {}[{}] skipped.".format(item.name, item.code))
				print(e)


@app.task
def update_news_data():
	all_news = StockNews.objects.all()
	for news in all_news:
		with transaction.atomic():
			try:
				d = get_news_data(news.item.name, news.most_recent_pub, news.keywords.split())
				news.keywords = d['keywords']
				if d['most_recent_pub']:
					if d['news_count'] != 0:
						news.news_count += d['news_count']
					news.most_recent_pub = d['most_recent_pub']
				else:
					news.news_count = d['news_count']
				news.save()
			except Exception as e:
				print("[-] update_news_data(): {} skipped.".format(news.item.name))
				print(e)
	
