from item.utils import *
from item.models import StockItem, StockNews
from django.db import transaction
import pandas as pd

@transaction.atomic
def recover():
	df = pd.read_csv("data/data.csv")
	all_news = StockNews.objects.all()

	for news in all_news:
		previous_keyword = df[df.name == news.item.name].keywords.values
		if len(previous_keyword) > 0:
			news.keywords = previous_keyword[0]
		news.news_count = 0
		news.save()

