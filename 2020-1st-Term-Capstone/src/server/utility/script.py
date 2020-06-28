from gensim.models import Word2Vec
from news import *
import random


def clean_html(raw_html):
    cleanr = re.compile('<.*?>|\w+@[a-z]+[.][a-z]{2,3}[.][a-z]{2,3}|[\n\t]')
    return re.sub(cleanr, '', raw_html)

def get_article(article):
	title = article['title'].replace('"','').replace("'",'')
	originallink = article['originallink']
	link = article['link']
	publication = article['pubDate']

	pub = article['pubDate'][5:-6].split()
	pub[1] = str(MONTH_DICT[pub[1]])
	diff = datetime.now() - datetime.strptime(' '.join(pub), '%d %m %Y %H:%M:%S')

	limit_days = 60
	if diff.days > limit_days: 
		return ''
		
	try:
		html = requests.get(link)
		soup = BeautifulSoup(html.text, 'html.parser')
		content = soup.body.find_all("div", id="articleBodyContents")[0].text
		if 'function _flash_removeCallback() {}' in content:
			content = content.split('function _flash_removeCallback() {}')[1]
		content = clean_html(content)
		if '기자 ▶' in content:
		    contnet = content.split('기자 ▶')[0][:-4]
		#content = content.replace('다.','다.\n')
	except Exception as e:
		#print("[-] get_article(): {}\nOriginal Link: {}\nLink: {}\n".format(title, originallink, link))
		#print(e)
		return title

	return title + "\n" + content


def get_news_words(khaiii, keyword='', page=1, size=50):
	encText = urllib.parse.quote(keyword)
	url = "https://openapi.naver.com/v1/search/news.json?query=" + encText + \
		"&display=" + str(size) + "&sort=sim"
	request = urllib.request.Request(url)
	request.add_header("X-Naver-Client-Id",client_id)
	request.add_header("X-Naver-Client-Secret",client_secret)
	response = urllib.request.urlopen(request, context=context)
	decoded_result = json.loads(response.read().decode())
	articles = list(map(get_article, decoded_result["items"]))

	ret = []
	
	for article in articles:
		if not article:
			continue
		
		nouns = []
		for content in khaiii.analyze(article):
			if content.length < 2:
				continue
			content = content.__str__().split("\t")[1]
			nouns.extend([ s.split("/") for s in content.split(" + ") ])
		nouns = list(filter(check_nnp, nouns))
		nouns = list(map(get_noun, nouns))
		nouns = list(filter(check_valid, nouns))
		ret.append(nouns)

	return (ret, len(ret)) # 2차원 배열, 총 뉴스 기사글
	

def load_data(names):
	khaiii = KhaiiiApi()
	#tokenized_data = []
	total_news_count = 0
	fname = 'articles_%06d.txt' % random.randint(0,999999)
	f = open(fname, 'w')
	for name in names:
		news_words_set, news_count = get_news_words(khaiii, name)
		for words in news_words_set:
			f.write(' '.join(words))
		#tokenized_data.extend(news_words)
		total_news_count += news_count
		print("{}'s article #{}".format(name, news_count))
	f.close()
	#return (tokenized_data, total_news_count)
	return (fname, total_news_count)


def learn(data, outfile):
	from multiprocessing import cpu_count, Pool
	import numpy as np
	
	tokenized_data = []
	total_news_count = 0
	
	num_cores = cpu_count()
	print("Process Count:", num_cores)
	splited_arr = np.array_split(data, num_cores)
	
	pool = Pool(num_cores)
	result = pool.map(load_data, splited_arr)
	pool.close()
	pool.join()
	
	for data, count in result:
		#tokenized_data.extend(data)
		print(data)
		total_news_count += count

	print("Total News Count:", total_news_count)
	'''
	model = Word2Vec(sentences = tokenized_data, size = 100, window = 5, min_count = 5, workers = 4, sg = 0)
	model.save(outfile)
	
	return model
	'''
	return


def generate(infile, outfile):
	import time
	import pandas as pd

	start = time.time()

	df = pd.read_csv(infile)
	df = df.dropna()
	df = df[df.volume > 0]
	df = df.reset_index(drop=True)
	df = df.sort_values(by=['fluctuation','volume'], ascending=False)

	model = learn(df.name.values, outfile)
	#model = Word2Vec.load(outfile)
	# model.train([["hello", "world"]], total_examples=1, epochs=1)
	end = time.time()
	
	'''
	with open('duration.log', 'w') as f:
	    f.write("vocab length: {}".format(len(model.wv.vocab.keys())))
	    f.write("[+] Training Duration: {} sec".format(end-start))
	
	return model.wv
	'''
	return


def run(wv):
	while True:
		try:
			choice = int(input("1: most similarity\n2: similarity of 2 words\n3: quit\n>>"))
		except Exception as e:
			print(e)
			exit()
		
		if choice == 1:
			while True:
				keyword = input("\nEnter a keyword (q is quit): ")
				if keyword == 'q':
					break
				print("\nMost Similarity:")
				print(wv.most_similar(keyword))
		elif choice == 2:
			while True:
				keyword1 = input("First Keyword: ")
				keyword2 = input("Second Keyword: ")
				print(wv.similarity(keyword1, keyword2))
	return

if __name__ == '__main__':
	infile = 'data.csv'
	outfile = "word2vec_ver3.model"
	wv = generate(infile,outfile)
	#run(wv)
	 
	# article # 50 per item, Duration: 4913.097075223923 sec
