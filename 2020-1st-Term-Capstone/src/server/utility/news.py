import json, collections, ssl, re, random, time, requests
import urllib.request
from urllib.parse import *
from datetime import datetime

from bs4 import BeautifulSoup
from khaiii import KhaiiiApi

api = KhaiiiApi()

context = ssl._create_unverified_context()

client_id = 'KotX6oDuxIJ47loijWDI'
client_secret = 'oO5_uV2tSh'

KEYWORD_COUNT = 20

MONTH_DICT = {'Jan':1, 'Feb':2, 'Mar':3, 'Apr':4, 'May':5, 'Jun':6, 'Jul':7, 'Aug':8, 'Sep':9, 'Oct':10, 'Nov':11, 'Dec':12}


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


def get_news_nouns(name='', page=1, size=100, last_publication=None):
    encText = urllib.parse.quote(name)
    url = "https://openapi.naver.com/v1/search/news.json?query=" + encText + \
        "&display=" + str(size) + "&sort=sim"
    request = urllib.request.Request(url)
    request.add_header("X-Naver-Client-Id",client_id)
    request.add_header("X-Naver-Client-Secret",client_secret)
    response = urllib.request.urlopen(request, context=context)
    decoded_result = json.loads(response.read().decode())
    if last_publication:
        contents = list(map(get_article_title, decoded_result["items"], [ last_publication ] * len(decoded_result)))
    else:
        contents = list(map(get_article_title, decoded_result["items"]))
    pub_list = [pub for title, pub in contents if pub]
    if len(pub_list) == 0 and not last_publication:
        most_recent_pub = datetime.now()
    elif len(pub_list) > 0:
        most_recent_pub =  max(pub_list)
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
    if news_count == 0:
        return (previous_keywords, 0, None)

    keywords = get_noun_count(news_nouns)
    if previous_keywords:
        if len(previous_keywords) < KEYWORD_COUNT:
            previous_keywords.extend([ word for word in keywords if word not in previous_keywords ])
            return (previous_keywords, news_count, None)
    
    return (keywords, news_count, most_recent_pub)

