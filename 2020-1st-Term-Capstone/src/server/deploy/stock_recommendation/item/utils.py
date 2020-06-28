#-*- coding:utf-8 -*-
import pandas as pd
import json, collections, ssl, re, random, time, requests
import urllib.request
from urllib.parse import *
from datetime import datetime
from bs4 import BeautifulSoup
from khaiii import KhaiiiApi

from django.utils import timezone
from django.db import transaction
from item.models import StockItem, StockNews

api = KhaiiiApi()

context = ssl._create_unverified_context()

client_id = 'KotX6oDuxIJ47loijWDI'
client_secret = 'oO5_uV2tSh'

KEYWORD_COUNT = 20
MONTH_DICT = {'Jan':1, 'Feb':2, 'Mar':3, 'Apr':4, 'May':5, 'Jun':6, 'Jul':7, 'Aug':8, 'Sep':9, 'Oct':10, 'Nov':11, 'Dec':12}

########################################## STOCK

def get_code_df():
    code_df = pd.read_html('http://kind.krx.co.kr/corpgeneral/corpList.do?method=download&searchType=13', header=0)[0]
    code_df.종목코드 = code_df.종목코드.map('{:06d}'.format)
    code_df = code_df[['회사명', '종목코드']]
    code_df = code_df.rename(columns={'회사명': 'name', '종목코드': 'code'})
    return code_df


def get_fixing_data(code):
    url = "https://navercomp.wisereport.co.kr/v2/company/c1010001.aspx?cmp_cd=" + code
    html = requests.get(url)
    if "Error" in html.url or html.status_code != 200:
        return None
    soup = BeautifulSoup(html.text, 'html.parser')
    body = soup.body
    table = body.find_all("td", class_="td0101")[0]
    wics = table.find_all("dt", class_="line-left")[2].contents[0].split(' ')[-1] # 업종
    return { 'wics': wics }


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
        'now_val': now_val, # 현재가
        'diff': diff, # 전일비
        'fluctuation': fluctuation, # 등락률
        'volume': volume, # 거래량
    }


########################################## NEWS

def get_article_title(article, most_recent_pub=None):
    pub = article['pubDate'][5:-6].split()
    pub[1] = str(MONTH_DICT[pub[1]])
    publication = datetime.strptime(' '.join(pub), '%d %m %Y %H:%M:%S')

    if most_recent_pub:
        if publication < most_recent_pub:
            return ('', None)
    else:
        diff = datetime.now() - publication
        if diff.days > 60: # 두달 전 기사글은 제외
            return ('', None)

    return (article['title'].replace('"','').replace("'",''), publication)


def get_noun(str_with_pos):
    return str_with_pos[0]


def check_nnp(noun):
    return True if noun[1] == "NNP" or noun[1] == "NNG" else False


def check_valid(noun):
    if len(noun) < 2: return False
    try:
        int(noun)
        return False
    except ValueError:
        return True


def clean_html(raw_html):
    cleanr = re.compile('<.*?>|&([a-z0-9]+|#[0-9]{1,6}|#x[0-9a-f]{1,6});')
    cleantext = re.sub(cleanr, '', raw_html)
    return cleantext


def get_news_nouns(name='', page=1, size=50, last_publication=None):
    encText = urllib.parse.quote(name)
    url = "https://openapi.naver.com/v1/search/news.json?query=" + encText + \
        "&display=" + str(size) + "&sort=sim"
    request = urllib.request.Request(url)
    request.add_header("X-Naver-Client-Id",client_id)
    request.add_header("X-Naver-Client-Secret",client_secret)
    response = urllib.request.urlopen(request, context=context)
    decoded_result = json.loads(response.read().decode('utf-8'))
    if last_publication:
        contents = list(map(get_article_title, decoded_result["items"], [ last_publication ] * len(decoded_result)))
    else:
        contents = list(map(get_article_title, decoded_result["items"]))
    pub_list = [pub for title, pub in contents if pub]
    most_recent_pub = max(pub_list) if len(pub_list) > 0 else timezone.now()
    contents = '\n'.join([title for title, pub in contents])

    nnps = []
    for desc in api.analyze(contents):
        if desc.length < 2: continue
        desc = desc.__str__().split("\t")[1]
        nnps.extend([ s.split("/") for s in desc.split(" + ") ])

    nnps = list(filter(check_nnp, nnps))
    nouns = list(map(get_noun, nnps))
    nouns = list(filter(check_valid, nouns))	

    return (nouns, len(contents), most_recent_pub)
	

def get_noun_count(nouns):
    count = collections.Counter(nouns)
    most_nouns = count.most_common(KEYWORD_COUNT)
    most_nouns = list(map(lambda elem: elem[0], most_nouns))
    return most_nouns


def get_keywords(name, last_publication=None, previous_keywords=None):
    news_nouns, news_count, most_recent_pub = get_news_nouns(name, last_publication)
    keywords = get_noun_count(news_nouns)
    if previous_keywords == None: previous_keywords = []
    if set(keywords + previous_keywords) == set(previous_keywords):
        return (previous_keywords, 0, None)

    if previous_keywords:
        if len(previous_keywords) < KEYWORD_COUNT:
            previous_keywords.extend([ word for word in keywords if word not in previous_keywords ])
            return (previous_keywords, news_count, None)
    
    return (keywords, news_count, most_recent_pub)


def get_news_data(name, last_publication=None, previous_keywords=None):
    try:
        news_result, news_count, most_recent_pub = get_keywords(name, last_publication, previous_keywords)
        keywords = ' '.join(news_result)
        if len(news_result) > 0:
            print("{}'s keywords: {}".format(name, keywords))
    except Exception as e:
        keywords = ''
        news_count = 0
        most_recent_pub = timezone.now()
        print('[-] get_news_data():', name, e)

    return {
        'keywords': keywords,
        'news_count': news_count,
        'most_recent_pub': most_recent_pub,
    }


########################################## GENERATOR

@transaction.atomic
def create_object(d):
    item = StockItem.objects.create(pk=d['code'],
                                    name=d['name'], wics=d['wics'],
                                    now_val=d['now_val'],
                                    fluctuation=d['fluctuation'],
                                    diff=d['diff'],
                                    volume=d['volume'])
    
    StockNews.objects.create(item=item,
                            keywords=d['keywords'],
                            news_count=d['news_count'])


def generate():
    start = time.time()
    code_df = get_code_df()
    i = 0
    for data in code_df.values:
        d = { "name":data[0], "code":data[1] }
        try:
            tmp = get_fixing_data(d['code'])
            if tmp == None: continue
            d.update(tmp)
            d.update(get_changing_data(d['code']))
            d.update(get_news_data(d['name']))
            create_object(d)
            #print("[+] generate(): ({},{})\t{}-th".format(d['name'], d['code'], i))
        except Exception as e:
            print("[-] generate(): failed ({},{})".format(d['name'], d['code']))
            print(e)
            continue
        i += 1
    end = time.time()
    print("[+] generate(): stock #{} spent time {} sec.".format(i, end-start))
