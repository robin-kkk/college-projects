import pandas as pd
import numpy as np
import random
import requests

from datetime import datetime
from multiprocessing import Process, Queue, Pool
from bs4 import BeautifulSoup

columns = ['name', 'code', 'wics', 'now_val', 'fluctuation', 'diff', 'volume', 'keywords', 'news_count', 'most_recent_pub']

def get_code_df():
	code_df = pd.read_html('http://kind.krx.co.kr/corpgeneral/corpList.do?method=download&searchType=13', header=0)[0]
	code_df.종목코드 = code_df.종목코드.map('{:06d}'.format)
	code_df = code_df[['회사명', '종목코드']]
	code_df = code_df.rename(columns={'회사명': 'name', '종목코드': 'code'})
	print("[+] get_code_df(): Code Shape =", code_df.shape)
	return code_df


def get_fixing_data(code):
	url = "https://navercomp.wisereport.co.kr/v2/company/c1010001.aspx?cmp_cd=" + code
	html = requests.get(url)
	if "Error" in html.url or html.status_code != 200:
		return None
	soup = BeautifulSoup(html.text, 'html.parser')
	body = soup.body
	table = body.find_all("td", class_="td0101")[0]
	wics = table.find_all("dt", class_="line-left")[2].contents[0].split(' ')[-1]
	return {'wics': [wics]}


# 실시간 변동 데이터
def get_changing_data(code):
	url = "https://finance.naver.com/item/sise.nhn?code=" + code
	html = requests.get(url)
	assert html.status_code == 200
	soup = BeautifulSoup(html.text, 'html.parser')
	table = soup.find_all("table", class_="type2")[0]

	now_val = int(table.find_all("strong", id="_nowVal")[0].contents[0].replace(',',''))
	diff = int(table.find_all("strong", id="_diff")[0].find_all("span", class_="tah")[0].contents[0].split()[0].replace(',',''))
	fluctuation = float(table.find_all("strong", id="_rate")[0].find_all("span", class_="tah")[0].contents[0].split()[0][:-1])
	if fluctuation < 0: diff *= -1
	volume = int(table.find_all("span", id="_quant")[0].contents[0].replace(',',''))

	return {
		'now_val': [now_val],
		'diff': [diff],
		'fluctuation': [fluctuation],
		'volume': [volume],
	}


def get_news_data(name, last_publication=None, previous_keywords=None):
	from news import get_keywords
	try:
		news_result, news_count, most_recent_pub = get_keywords(name, last_publication, previous_keywords)
		keywords = ' '.join(news_result)
		if len(news_result) > 0:
			print("{}'s keywords: {}".format(name, keywords))
		most_recent_pub = datetime.strftime(most_recent_pub, "%Y-%m-%d %H:%M:%S")
	except Exception as e:
		keywords = ''
		news_count = 0
		most_recent_pub = None
		print('[-] get_news_data():', name, e)

	return {
		'keywords': [keywords],
		'news_count': [news_count],
		'most_recent_pub': [most_recent_pub]
	}


def process_data(code_df):
	df = pd.DataFrame(columns=columns)
	
	i = 1
	for data in code_df.values:
		name = data[0]
		code = data[1]
		try:
			d = {"name":[name], "code":[code]}
			tmp = get_fixing_data(code)
			if tmp == None: continue
			d.update(tmp)
			d.update(get_changing_data(code))
			d.update(get_news_data(name))
			df = pd.concat([df,pd.DataFrame(d)], ignore_index=True, sort=False)
			print("[+] gather_data(): ({},{}) is parsed. - {} -".format(name, code, i))
		except Exception as e:
			print("[-] gather_data(): not found ({},{})".format(name,code))
			print(e)
			exit()
		i += 1
		
	return df


def gather_data(code_df):
	process_count = 4
	splited_arr = np.array_split(code_df, process_count)
	pool = Pool(process_count)
	df = pd.concat(pool.map(process_data, splited_arr))
	pool.close()
	pool.join()
	return df


def generate(filepath):
	import time
	start = time.time()

	#df = gather_data(get_code_df())
	df = process_data(get_code_df())
	df.to_csv(filepath, index=False)

	end = time.time()
	print("Duration: {} sec".format(end - start));

if __name__ == "__main__":
	generate('./data.csv') # Duration: 2383.256291627884 sec , 1153.3718814849854 sec
	#update('./data.csv')
	
