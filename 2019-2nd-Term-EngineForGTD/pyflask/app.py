# -*- coding: utf-8 -*-
from flask import Flask, render_template, request, abort, jsonify, Response
from werkzeug.utils import secure_filename
from datetime import datetime
import time
import pandas as pd

app = Flask(__name__)

data_path = './static/data/'

@app.route('/', methods=['GET', 'POST'])
def index():
	if request.method == 'GET':
		return render_template('index.html', status=-1)

	f = request.files['csvfile']
	today = datetime.strftime(datetime.now(), '%Y%m%d%H%M%S')
	fname = today + "-" + secure_filename(f.filename)
	f.save(data_path + fname)
	app.logger.info('Saved as - %s', fname)
	return jsonify({'csvname': fname})


@app.route('/next', methods=['GET', 'POST'])
def do_basic_analysis():
	if request.method == 'GET':
		abort(404)
	fname = request.form.get('upload-name')
	app.logger.info('filename in next: %s', fname)
	csvpath = data_path + fname

	try:
		df = pd.read_csv(csvpath, encoding="ISO-8859-1")
		blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
		df.drop(blank_columns, axis=1, inplace=True)

		null_info = list(df.isnull().sum())
		cnt = df.shape[0]
		cols = df.shape[1]
		boundary = cnt / 2
		dropped_columns = []
		dropped_null_ratio = []
		filtered_columns = []
		filtered_null_ratio = []
		selectable_features = []

		for i in range(len(null_info)):
			ratio = null_info[i] / cnt * 100
			if null_info[i] > boundary:
				dropped_columns.append(df.columns[i])
				dropped_null_ratio.append(ratio)
			else:
				filtered_columns.append(df.columns[i])
				filtered_null_ratio.append(ratio)
				if df.columns[i] in ['scite1','scite2', 'summary']: # not category
					continue
				if not df.columns[i].endswith('txt'):
					selectable_features.append(df.columns[i])

		df.drop(dropped_columns, axis=1, inplace=True)
		new_path = csvpath[:-4] + "-filtered.csv"
		df.to_csv(new_path, encoding="ISO-8859-1")
		app.logger.info('%s preprocessed!', new_path)
	except Exception as e:
		app.logger.info(e)
		return render_template('index.html', status=0)
	
	result = {
				'rows': cnt,
				'cols': cols,
				'dropped_cols': dropped_columns,
				'dropped_ratio': dropped_null_ratio,
				'filtered_cols': filtered_columns,
				'filtered_ratio': filtered_null_ratio,
				'selectable': selectable_features,
				'csvname': fname,
			}
	return render_template('index.html', status=1, data=result)


@app.route('/analysis', methods=['GET', 'POST'])
def analyze():
	if request.method == 'GET':
		abort(404)

	try:
		csvname = request.form.get('csvname')[:-4] + "-filtered.csv"
		csvpath = data_path + csvname
		app.logger.info("csvpath in analyze(): %s", csvpath)
		feature_group = request.form.get('feature_group').split('-')
		checked_group = request.form.get('checked_group').split(',')
		features_list = []
		checked_list = []
		for i in range(len(feature_group)):
			l = feature_group[i].split(',')
			if l[0]:
				features_list.append(l)
				checked_list.append(True if checked_group[i][0] == 't' else False)

		assert len(features_list) == len(checked_list)

		df = pd.read_csv(csvpath)
		blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
		df.drop(blank_columns, axis=1, inplace=True)
		
		from constant import values
		from utils import do_mca, elbow
		
		dirpath = data_path + 'images/'
		mca_img_fname =	[]
		mca_result_fname = []
		elbow_img_fname = []
		today = datetime.strftime(datetime.now(), '%Y%m%d%H%M%S')
		for i in range(len(features_list)):
			sample = df[features_list[i]].copy()
			if checked_list[i]: # Remove Missing Values 체크한 경우
				sample.dropna(inplace=True)
			else:
				sample.fillna(value=values, inplace=True)
			
			filename = "%s-g%d-mca-of-%s.png" % (today, i+1, csvname[:-4])
			mca_img_fname.append(filename)
			rst, dataset = do_mca(
				sample,
				imgpath=dirpath + filename,
				showing_option=[True, False, False, False]
			) # row_coordinates
			if not rst:
				abort(404)
				app.logger.info("Error in do_mca()")
				break

			print("dataset shape after mca: ", dataset.shape)
			mca_csv_name = filename[:-4] + ".csv"
			dataset.to_csv(data_path + mca_csv_name, encoding="ISO-8859-1")
			mca_result_fname.append(mca_csv_name)

			filename = filename.replace('-mca-', '-elbow-')
			if not elbow(dataset, dirpath + filename):
				abort(404)
				app.logger.info("Error in elbow()")
				break
			elbow_img_fname.append(filename)
	
	except Exception as e:
		app.logger.info(e)
		abort(404)

	result = {
		'csvname': csvname,
		'mca_result_fname': ','.join(mca_result_fname),
		'mca_img_fname': ','.join(mca_img_fname),
		'elbow_img_fname': ','.join(elbow_img_fname),
		'features_list': '-'.join([ ','.join(f) for f in features_list ]),
		'checked_list': ','.join(list(map(str, checked_list))),
	}

	return jsonify(result)


@app.route('/clustering', methods=['GET', 'POST'])
def k_select_page():
	if request.method == 'GET':
		abort(404)

	csvname = request.form.get('csvname')
	csvpath = data_path + csvname
	app.logger.info("csvpath in select_k(): %s", csvpath)
	mca_result_fname = request.form.get('mca_result_fname')
	mca_img_fname = request.form.get('mca_img_fname').split(',')
	elbow_img_fname = request.form.get('elbow_img_fname').split(',')
	features_list = request.form.get('features_list').split('-')
	checked_list = request.form.get('checked_list')

	result = {
		'csvname': csvname,
		'mca_result_fname': mca_result_fname, # csvfile
		'mca_img_fname': mca_img_fname,
		'elbow_img_fname': elbow_img_fname,
		'features_list': features_list,
		'checked_list': checked_list,
	}

	return render_template('analysis.html', data=result)


@app.route('/analysis/silhouette', methods=['GET', 'POST'])
def evaluator():
	if request.method == 'GET':
		abort(404)

	try:
		k = int(request.form.get('k'))
		csvname = request.form.get('csvname')
		csvpath = data_path + csvname
		
		df = pd.read_csv(csvpath)
		blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
		df.drop(blank_columns, axis=1, inplace=True)

		imgpath = data_path + "images/" + csvname.replace('-mca-', '-mca-silhouette-')[:-4] + ".png"

		from utils import silhouette
		if not silhouette(df, k, imgpath):
			imgpath = -1

	except Exception as e:
		app.logger.info(e);
		abort(404)

	result = {
		'imgpath': imgpath
	}

	return jsonify(result)


@app.route('/clustering/kmeans', methods=['GET', 'POST'])
def clustering():
	if request.method == 'GET':
		abort(404)

	csvname = request.form.get('csvname')
	csvpath = data_path + csvname
	app.logger.info("csvpath in clustering(): %s", csvpath)
	mca_result_fname = request.form.get('mca_result_fname').split(',')
	features_list = request.form.get('features_list').split('-')
	k_list = request.form.get('k_list').split(',')

	assert len(mca_result_fname) == len(k_list)
	assert len(features_list) == len(k_list)

	from utils import kmeans
	from constant import values
	
	clustering_result_csvnames = []
	before_img_names = []
	after_img_names = []
	
	for (k, ds_fname, f) in zip(k_list, mca_result_fname, features_list):
		fname = ds_fname[:-4]
		output_before = fname + "-before-kmeans.png"
		output_after = fname + "-after-kmeans.png"
		img_names = [output_before, output_after]
		before_img_names.append(output_before)
		after_img_names.append(output_after)

		rst = kmeans(int(k), data_path + ds_fname, img_names)
		assert rst == True

	result = {
		'csvname': csvname,
		'before_img_names': ','.join(before_img_names),
		'after_img_names': ','.join(after_img_names),
		'clustering_result_csvnames': ','.join(mca_result_fname),
	}
	return jsonify(result)


@app.route('/clustering/kmeans/result', methods=['GET', 'POST'])
def result():
	if request.method == 'GET':
		abort(404)

	csvname = request.form.get('csvname')
	before_img_names = request.form.get('before_img_names').split(',')
	after_img_names = request.form.get('after_img_names').split(',')
	clustering_result_csvnames = request.form.get('clustering_result_csvnames').split(',')
	features_list = [ f.replace(' ','').split(',') for f in request.form.get('features_list').split('-') ]
	app.logger.info('features_list: ',features_list)
	checked_list = request.form.get('checked_list').split(',')
	checked_list = list(map(lambda x: True if x[0] == 'T' else False, checked_list))

	assert len(features_list) == len(checked_list)
	assert len(before_img_names) == len(features_list)
	assert len(before_img_names) == len(after_img_names)
	assert len(before_img_names) == len(clustering_result_csvnames)

	from constant import values
	from operator import itemgetter
	from utils import visualization, save_dist_by_something


	df = pd.read_csv(data_path + csvname)
	blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
	df.drop(blank_columns, axis=1, inplace=True)
	
	result = {}
	
	for (before, after, result_fname, features, checked, n) in \
	zip(before_img_names, after_img_names, clustering_result_csvnames,
		features_list, checked_list, range(len(features_list))):
		
		prefix = result_fname.split('-')[0]
		key = 'group'+str(n+1)
		result[key] = {
			'before': before,
			'after': after,
			'cluster': [],
			'cluster_ratio': [],
			'dist_by_country': [],
			'dist_by_region': [],
			'visualization_list': [],
			'cluster_set_name': [],
			'feature_list': features_list[n]
		}
		
		dataset = df.copy()
		if checked:
			dataset.dropna(subset=features, inplace=True)
		dataset.fillna(value=values, inplace=True)
		dataset.reset_index(inplace=True)
		if dataset.columns[0] == 'index':
			dataset.drop(['index'], axis=1, inplace=True)

		clustering_set = pd.read_csv(data_path + result_fname)

		assert dataset.shape[0] == clustering_set.shape[0]
		
		dataset = pd.concat([dataset, clustering_set[['cluster_id']]], axis=1)
		
		cluster_ids = list(dataset.cluster_id.unique())
		cluster_ids.sort()

		total_cnt = int(dataset.shape[0])

		visual_names = []
		for c_id in cluster_ids:
			cluster_set = dataset[dataset.cluster_id==c_id]
			cluster_cnt = int(cluster_set.shape[0])
			result[key]['cluster_ratio'].append(round(int(cluster_set.shape[0])/total_cnt * 100, 3))
			feature_dist = {}
			for feature in features:
				feature2 = feature
				if feature + "_txt" in cluster_set.columns:
					column_data = cluster_set[feature + "_txt"]
					feature2 = feature + "_txt"
				else:
					column_data = cluster_set[feature]
					feature2 = feature

				coun_feature = round(cluster_set[feature2].value_counts()/cluster_cnt*100, 3).to_frame()
				coun_feature['category'] = list(coun_feature.index)
				coun_feature.reset_index(inplace=True, drop=True)
				coun_feature.columns = ['attacks', 'category']
				feature_dist[feature] = coun_feature.values.tolist()

			
			result[key]['cluster'].append(feature_dist)
			filename = "%s-%s-cluster-%s-dist-by-country-of-%s.png" % (prefix, key, c_id, csvname[:-4])
			save_dist_by_something(cluster_set, data_path + "images/" + filename, "country_txt")
			result[key]['dist_by_country'].append(filename)

			filename = "%s-%s-cluster-%s-dist-by-region-of-%s.png" % (prefix, key, c_id, csvname[:-4])
			save_dist_by_something(cluster_set, data_path + "images/" + filename, "region_txt")
			result[key]['dist_by_region'].append(filename)

			filename = "%s-%s-cluster-%s-of-%s.html" % (prefix, key, c_id, csvname[:-4])
			row_cnt_diff = visualization(cluster_set, filename)
			result[key]['visualization_list'].append(filename)
			result[key]['row_cnt_diff'] = 0 #row_cnt_diff

			cluster_set_name = result_fname[:-4] + "-cluster-" + str(c_id+1) + ".csv"
			result[key]['cluster_set_name'].append(cluster_set_name)
			cluster_set.to_csv(data_path + cluster_set_name, encoding="ISO-8859-1")

	# app.logger.info(result)

	return render_template('result.html', data=result, csvname=csvname)


@app.route('/download', methods=['GET', 'POST'])
def download():
	if request.method == 'GET':
		abort(404)

	from io import StringIO

	csvname = request.form.get('csvname').replace('-filtered','')
	export_name = request.form.get('dataclass')
	cluster_set_name = request.form.get('cluster_set_name')

	app.logger.info('csvname: %s', csvname)
	app.logger.info('export_name: %s', export_name)
	app.logger.info('cluster_set_name: %s', cluster_set_name)

	df = pd.read_csv(data_path + csvname)
	blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
	df.drop(blank_columns, axis=1, inplace=True)
	cluster_set = pd.read_csv(data_path + cluster_set_name)
	new_csv = df[df.eventid.isin(cluster_set.eventid)]
	new_csv.reset_index(inplace=True, drop=True)
	output_stream = StringIO()
	new_csv.to_csv(output_stream, encoding="ISO-8859-1")
	response = Response(
		output_stream.getvalue(), 
		mimetype='text/csv',
		content_type='application/octet-stream',
	)
	response.headers["Content-Disposition"] = "attachment; filename="+export_name+".csv"
	return response
