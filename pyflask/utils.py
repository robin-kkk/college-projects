from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_samples
import seaborn as sns
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import rcParams
from matplotlib import cm


sns.set(rc={'figure.figsize':(11.7,8.27)})
rcParams['figure.figsize'] = 11.7,8.27

def do_mca(dataset, imgpath, showing_option=[True, True, False, False]):
	try:
		import prince
		mca = prince.MCA(
			n_components=2,
			n_iter=3,
			copy=True,
			check_input=True,
			engine='auto',
			random_state=42
		)
		mca = mca.fit(dataset)
		mca.plot_coordinates(
			X=dataset,
			ax=None,
			figsize=(15, 15),
			show_row_points=showing_option[0],
			row_points_size=10,
			show_row_labels=showing_option[1],
			show_column_points=showing_option[2],
			column_points_size=10,
			show_column_labels=showing_option[3],
			legend_n_cols=1,
		).get_figure().savefig(imgpath)
		
		print ("Total inertia : ", mca.total_inertia_)
		print ("eigenvalues : ", mca.eigenvalues_)
		print ("explained_inetria: ", mca.explained_inertia_)
		print ()

		result = None
		if showing_option[0]:
			result = mca.row_coordinates(dataset.copy())
		else:
			result = mca.column_coordinates(dataset.copy())

	except:
		return False, None
	
	return True, result


def elbow(dataset, imgpath):
	try:
		sse = []
		for i in range(1, 21):
			km = KMeans(n_clusters=i, init='k-means++', random_state=0).fit(dataset)
			sse.append(km.inertia_)
		
		plt.plot(range(1, 21), sse, marker='o')
		plt.xlabel='number of clusters'
		plt.ylabel='SSE'
		plt.savefig(imgpath)
	except:
		return False
	return True


def silhouette(dataset, number_of_clusters, imgpath):
	try:
		plt.clf()
		km = KMeans(n_clusters=number_of_clusters, random_state=0).fit_predict(dataset)
		cluster_labels = np.unique(km)
		n_clusters = cluster_labels.shape[0]
		silhouette_vals = silhouette_samples(dataset, km, metric='euclidean')
		y_ax_lower, y_ax_upper = 0,0
		yticks = []

		for i, c in enumerate(cluster_labels):
			c_silhouette_vals = silhouette_vals[km == c]
			c_silhouette_vals.sort()
			y_ax_upper += len(c_silhouette_vals)
			color = cm.jet(i/n_clusters)

			plt.barh(
				range(y_ax_lower, y_ax_upper),
				c_silhouette_vals,
				height=1.0,
				edgecolor='none',
				color=color
			)

			yticks.append((y_ax_lower + y_ax_upper) / 2)
			y_ax_lower += len(c_silhouette_vals)

		silhouette_avg = np.mean(silhouette_vals)
		plt.axvline(silhouette_avg, color='red', linestyle='--')
		plt.yticks(yticks, cluster_labels+1)
		plt.xlabel = 'Cluster'
		plt.xlabel = 'Silhouette Coefficient'
		plt.savefig(imgpath)
	except:
		return False
	return True

def kmeans(k, csvpath, outputs, title=['Before KMeans', 'After KMeans']):
	try:
		df = pd.read_csv(csvpath)
		blank_columns = [ c for c in df.columns if c.startswith('Unnamed:')]
		df.drop(blank_columns, axis=1, inplace=True)

		df.columns = ['component1', 'component2']
		sns.lmplot(
			'component1', 'component2', # x-axis, y-axis,
			data=df, fit_reg=False, # data, no line,
			scatter_kws={"s": 5} # marker size
		)

		plt.title(title[0])
		plt.xlabel = 'component1'
		plt.ylabel = 'component2'
		plt.savefig('static/data/images/' + outputs[0])

		kmeans = KMeans(n_clusters=k).fit(df.values)
		df['cluster_id'] = kmeans.labels_

		sns.lmplot(
			'component1', 'component2', # x-axis, y-axis,
			data=df, fit_reg=False, # data, no line
			scatter_kws={"s": 5}, # marker size
			hue="cluster_id" # color
		)

		plt.title(title[1])
		plt.savefig('static/data/images/' + outputs[1])
		df.to_csv(csvpath)
	except Exception as e:
		print(e)
		return False
	return True
	

def visualization(dataset, filename):
	import folium
	import folium.plugins

	if len(dataset) > 5000:
		terror_fol = dataset.sample(n=5000).copy()
	else:
		terror_fol = dataset.copy()
	
	row_count_diff = terror_fol.shape[0]
	terror_fol.dropna(subset=['latitude','longitude'],inplace=True)
	row_count_diff -= terror_fol.shape[0]
	location_fol=terror_fol[['latitude','longitude']]
	year_fol=terror_fol['iyear']
	month_fol=terror_fol['imonth']
	day_fol=terror_fol['iday']
	country_fol=terror_fol['country_txt']
	city_fol=terror_fol['city']
	killed_fol=terror_fol['nkill']
	wound_fol=terror_fol['nwound']
	attack_fol = terror_fol['attacktype1_txt']
	target_fol = terror_fol['targtype1_txt']
	targetsub_fol = terror_fol['targsubtype1_txt']
	weapon_fol = terror_fol['weaptype1_txt']
	weaponsub_fol = terror_fol['weapsubtype1_txt']
	cluster_id = terror_fol['cluster_id']

	marker_style = [
		'darkred', 'blue', 'orange', 'green', 'purple', 'red', 'pink',
		'cadetblue', 'darkgreen', 'pink', 'gray', 'darkblue'
	]

	map2 = folium.Map(location=[30,0],tiles='CartoDB positron',zoom_start=2) # dark_matter

	for point in location_fol.index:
		info = "<b>Date: </b>%4d.%2d.%2d<br> \
				<b>Country: </b>%s<br> \
				<b>City: </b>: %s<br> \
				<b>Killed </b>: %d <br> \
				<b>Wounded</b>: %d <br> \
				<b>Attack Type </b>: %s <br> \
				<b>Target Type </b>: %s <br> \
				<b>Target SubType </b>: %s <br> \
				<b>Weapon Type </b>: %s <br> \
				<b>Weapon SubType </b>: %s <br> " % (year_fol[point], month_fol[point],
					day_fol[point], country_fol[point],
					city_fol[point], killed_fol[point],
					wound_fol[point], attack_fol[point],
					target_fol[point], targetsub_fol[point],
					weapon_fol[point], weaponsub_fol[point])

		iframe = folium.IFrame(html=info, width=200, height=200)
		folium.CircleMarker(
			list(location_fol.loc[point].values),
			popup=folium.Popup(iframe),
			radius=1,
			color=marker_style[int(cluster_id[point])]
		).add_to(map2)

	map2.save('static/data/html/' + filename)

	return row_count_diff


def save_dist_by_something(dataset, imgpath, something):
	plt.clf()
	terror = dataset[dataset['nkill'] > 0]
	coun_terror=terror[something].value_counts()[:15].to_frame()
	coun_terror.columns=['Attacks']
	coun_kill=terror.groupby(something)['nkill'].sum().to_frame()
	coun_kill.columns=['Killed']
	coun_terror.merge(coun_kill,left_index=True,right_index=True,how='left').plot.bar(width=0.9)
	fig=plt.gcf()
	fig.set_size_inches(18,6)
	fig.savefig(imgpath, bbox_inches='tight')
