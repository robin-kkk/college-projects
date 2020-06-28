from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

def count_vectorizer(keywords):
	vectorizer = CountVectorizer(ngram_range=(1, 3))
	X = vectorizer.fit_transform(keywords)
	return cosine_similarity(X, X).argsort()[:,::-1]


def tfidf_vectorize(keywords):
	tfidf_vectorizer = TfidfVectorizer()
	X = tfidf_vectorizer.fit_transform(keywords)
	return cosine_similarity(X, X).argsort()[:,::-1]


def recommend_by_specific_item(df, c_sim, keywords, top=10):
    # 특정 종목과 유사한 종목 추천 (10개)
	target_index = df[df['keywords'] == keywords].index.values
	sim_index = c_sim[target_index, :top].reshape(-1)
	sim_index = sim_index[sim_index != target_index]
	result = df.iloc[sim_index].sort_values('volume', ascending=False)
	return result


def recommend_by_popularity(df, c_sim, is_random=False, top=5):
	if is_random:
		# 무작위 추천 (Top 100 에서 무작위로 10개 뽑기)
		import random
		target_index = df[:100].index.values
		random.shuffle(target_index)
		target_index = target_index[:10]
	else:
		# 인기있는 상승주 Top 10 과 연관된 종목 추천
		target_index = df[:10].index.values
	
	sim_index = c_sim[target_index, :top].reshape(-1) # 각 종목별로 유사한 종목을 5개까지 가져옴
	# 자기 자신 제외, 10개의 종목마다 5개씩 추천: (50 - 5) = 45
	for idx in target_index:
		sim_index = sim_index[sim_index != idx]
	
	result = df.iloc[sim_index].sort_values('volume', ascending=False)
	return result


def recommend(infile):
	import pandas as pd
	df = pd.read_csv(infile)
	df = df.dropna()
	df = df[df.volume > 0] # 거래량 없는건 제외
	df = df.reset_index(drop=True)

	# 인기있는 상승주 (등락률, 거래량) 기준으로 정렬
	df = df.sort_values(by=['fluctuation','volume'], ascending=False)
	
	c_sim = count_vectorizer(df.keywords)
	#c_sim = tfidf_vectorize(df.keywords)

	#items = recommend_by_specific_item(df,c_sim)
	#items = recommend_by_popularity(df,c_sim)
	items = recommend_by_popularity(df,c_sim,is_random=True)
	return items


def run(infile, outfile):
	import time
	start = time.time()
	output = recommend(infile)
	output.to_csv(outfile, index=False)
	end = time.time()
	print("Duration: {} sec".format(end - start))

if __name__ == '__main__':
	run("data.csv", "list_from_single_tfidf.csv")

