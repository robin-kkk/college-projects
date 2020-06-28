import json
import random

from gensim.models import Word2Vec
from sklearn.metrics.pairwise import cosine_similarity

from django.http import Http404, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from .models import StockItem, StockNews

MODEL_PATH = "data/word2vec_ver2.model"
model = Word2Vec.load(MODEL_PATH)
vocabs = model.wv.vocab.keys()

##############################################################  UTILITY

def response(data):
    return HttpResponse(json.dumps(data, ensure_ascii=False))

def vectorize(keywords):
    # from sklearn.feature_extraction.text import TfidfVectorizer
    # vectorizer = TfidfVectorizer()
    from sklearn.feature_extraction.text import CountVectorizer
    vectorizer = CountVectorizer(ngram_range=(1, 3))
    X = vectorizer.fit_transform(keywords)
    return cosine_similarity(X, X).argsort()[:,::-1]

def get_news_set(min_stock_price=3000):
    news = StockNews.objects.filter(item__volume__gt=0, item__now_val__gt=min_stock_price, item__fluctuation__gt=0.0)
    news = news.order_by('-item__fluctuation', '-item__volume')
    return news
    
def rmd_by_keyword(news_set, target, stock_code=None):
    scores = {}
    for news in news_set:
        if news.item.code == stock_code:
            continue
        avg_score = 0.0
        cnt = 0
        for keyword in news.keywords.split():
            for compared in target:
                if compared not in vocabs or keyword not in vocabs:
                    continue
                avg_score += model.wv.similarity(compared, keyword)
                cnt += 1
        if cnt == 0:
            continue
        avg_score /= cnt
        scores[news.item.code] = avg_score
    
    if len(scores) == 0:
        return []

    rmd_cnt = 10
    filtered = [ code for code, score in sorted(scores.items(), key=(lambda x: x[1]), reverse=True)[:rmd_cnt] ]
    return [ item.convert_to_dict() for item in StockItem.objects.filter(pk__in=filtered).order_by('-fluctuation') ]

############################################################## API

def rmd_by_hottest(request):
    news_set = get_news_set()
    keyword_list = [ v[0] for v in news_set.values_list('keywords') ]
    c_sim = vectorize(keyword_list)

    target_index = [ i for i in range(10) ]
    # 각 종목별로 유사한 종목을 5개까지 가져옴
    sim_index = c_sim[target_index, :6].reshape(-1)
    # 자기 자신 제외, 10개의 종목마다 5개씩 추천: (60 - 10) = 50
    for idx in target_index:
        sim_index = sim_index[sim_index != idx]
        
    rmd_list = [ news_set[i].item.convert_to_dict() for i in range(len(news_set)) if i in sim_index ]
    return response({'content':rmd_list})


def rmd_by_random(request):
    news_set = get_news_set()
    keyword_list = [ v[0] for v in news_set.values_list('keywords') ]
    c_sim = vectorize(keyword_list)
    
    # 무작위 추천 (Top 100 에서 무작위로 10개 뽑기)
    target_index = [ i for i in range(100) ]
    random.shuffle(target_index)
    target_index = target_index[:10]
    # 각 종목별로 유사한 종목을 5개까지 가져옴
    sim_index = c_sim[target_index, :6].reshape(-1)
    # 자기 자신 제외, 10개의 종목마다 5개씩 추천: (60 - 10) = 50
    for idx in target_index:
        sim_index = sim_index[sim_index != idx]
	
    rmd_list = [ news_set[i].item.convert_to_dict() for i in range(len(news_set)) if i in sim_index ]
    return response({'content':rmd_list})


def get_hottest(request):
    all_items = StockItem.objects.all().order_by('-volume', '-fluctuation')
    rmd_list = []
    for item in all_items[:100]:
        rmd_list.append(item.convert_to_dict())
    return response({'content':rmd_list})


def get_goldencross(request):
    import pandas as pd
    df = pd.read_html("http://finance.naver.com/sise/item_gold.nhn",
                    attrs={'class':'type_5'},
                    encoding='euc-kr',
                    skiprows=1)[0]

    df.drop(columns=['Unnamed: 0','Unnamed: 11'], axis=1, inplace=True)
    df.columns = ['name', 'now_val', 'diff', 'fluctuation', 'volume', 'hour_val', 'high', 'low', 'PER', 'ROE']
    df.dropna(axis=0,how='all', inplace=True)
    df.fillna(0,inplace=True)
    
    filtered = StockItem.objects.filter(name__in=df.name.values.tolist())[:15]
    rmd_list = [ item.convert_to_dict() for item in filtered ]
    return response({'content':rmd_list})


def rmd_by_default(request):
    news_set = get_news_set()
    default_tags = []
    cnt = 0
    for news in news_set:
        keywords = news.keywords.split()
        if len(keywords) < 4:
            continue
        default_tags.extend(keywords[:3])
        cnt += 1
        if cnt == 10:
            break
    
    rmd_list = rmd_by_keyword(news_set, default_tags)
    return response({
        'default_tags': default_tags,
        'content': rmd_list,
    })


@csrf_exempt
def rmd_by_tags(request):
    if request.method != 'POST':
        raise Http404('Not Found')
    body = json.loads(request.body)
    user_tags = body['tags'].split()
    rmd_list = rmd_by_keyword(get_news_set(), user_tags)
    return response({'content':rmd_list})


@csrf_exempt
def rmd_by_item(request):
    if request.method != 'POST':
        raise Http404('Not Found')
    body = json.loads(request.body)
    target = body['code']
    news_set = get_news_set()
    target_news = news_set.filter(item__pk=target)
    if not target_news.exists():
        raise Http404('Not Found')
    rmd_list = rmd_by_keyword(news_set, target_news[0].keywords.split(), target)
    return response({'content':rmd_list})
    
