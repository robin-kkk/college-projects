from flask import Flask, url_for, render_template, request, redirect, session, jsonify
from werkzeug.security import generate_password_hash, check_password_hash
from utils import *
from datetime import datetime
import random

app = Flask(__name__)
app.config['SECRET_KEY'] = 'KEY{s3cr3t_4lph4b3t}'


def check_session(rs):
	if 'session_key' not in session:
		return None
	artist_id = rs.open(session['session_key'])
	if artist_id is None:
		del session['session_key']
		return None
	return rs.get_data(artist_id)


@app.route('/', methods=['GET', 'POST'])
def home():
	r = RedisSession()
	user = check_session(r)
	
	total_albums = int(r.get('the_number_of_albums').decode())
	total_artists = int(r.get('the_number_of_artists').decode())

	all_keys = r.keys()
	all_album_id = sorted(list(filter(filter_album_id, all_keys)))
	all_keys.remove('the_number_of_albums')
	all_keys.remove('the_number_of_artists')
	all_artist_id = list(filter(filter_artist_id, all_keys))

	import random
	random.shuffle(all_artist_id)

	genre_list = []
	with open('dataset/album-genre.txt', 'r') as f:
		genre_list = f.read().split(', ')


	from constant import ALBUM_COVER_LIST
	all_albums = {}
	for album_id in all_album_id[:30]:
		album = r.get_data(album_id)
		album['cover'] = ALBUM_COVER_LIST[random.randint(0,len(ALBUM_COVER_LIST)-1)]
		all_albums[album_id] = album
	

	artist_rmd_list = {}
	album_list = []
	for artist_id in all_artist_id[:3]:
		artist = r.get_data(artist_id)
		collaborations = [ _id for _id, value in artist['request_list'].items() if filter_collaborators(value) ]
		collaborations.extend([ _id for _id, value in artist['response_list'].items() if filter_collaborators(value) ])
		
		artist_rmd_list[artist_id] = [artist['real_name'], artist['role'], artist['albums'], len(set(collaborations))]
		album_list.extend(artist_rmd_list[artist_id][2])
	
	album_list = list(set(album_list))
	album_rmd_list = {}
	for album_id in album_list:
		album = r.get_data(album_id)
		album_rmd_list[album_id] = album['album_title']
	
	

	data = {
		'genre_list': genre_list,
		'num_albums': len(all_album_id),
		'num_artists': len(all_artist_id),
		'artist_rmd_list': artist_rmd_list,
		'album_rmd_list': album_rmd_list,
		'all_albums': all_albums,
		'all_artists': sorted(all_artist_id),
	}

	if user is None:
		return render_template('index.html', data=data)
	
	return render_template('index.html', data=data, user=user)


@app.route('/load', methods=['GET', 'POST'])
def load():
	r = RedisSession()
	
	if request.method == 'GET':
		return render_template('404.html')
	
	selected_genre = request.form.get('selected_genre')
	last_album_id = request.form.get('last_album_id')

	all_keys = r.keys()
	all_album_id = sorted(list(filter(filter_album_id, all_keys)))

	from constant import ALBUM_COVER_LIST
	data = {}
	if selected_genre != 'all':
		cnt = 0
		for album_id in all_album_id:
			if int(album_id) <= int(last_album_id):
				continue
			album = r.get_data(album_id)
			if album['genre'] != selected_genre:
				continue
			data[album_id] = {
				'album_title': album['album_title'],
				'genre': album['genre'],
				'cover': ALBUM_COVER_LIST[random.randint(0,len(ALBUM_COVER_LIST)-1)],
			}
			cnt += 1
			if cnt == 30:
				break
	else:
		for album_id in all_album_id[:30]:
			album = r.get_data(album_id)
			data[album_id] = {
				'album_title': album['album_title'],
				'genre': album['genre'],
				'cover': ALBUM_COVER_LIST[random.randint(0,len(ALBUM_COVER_LIST)-1)],
			}
			
	return jsonify(data)


@app.route('/register', methods=['GET', 'POST'])
def register():
	data = {}
	with open('dataset/user-role.txt', 'r') as f1, \
		open('dataset/country.txt', 'r') as f2, \
		open('dataset/city.txt', 'r') as f3:
		data['role'] = f1.read().split(', ')
		data['country'] = f2.read().split(', ')
		data['city'] = f3.read().split(', ')

	if request.method == 'POST':
		artist_id = request.form['artist_id']
		password = request.form['password']
		real_name = request.form['real_name']
		art_name = request.form['art_name']
		role = request.form['role']
		year_of_birth = request.form['year_of_birth']
		country = request.form['country']
		city = request.form['city']
		email = request.form['email']

		r = RedisSession()
		if r.get(artist_id):
			msg = 'This Artist ID, {}, already exists.'.format(artist_id)
			return render_template('register.html', data=data, msg=msg)

		user = {
			'artist_id':artist_id,
			'password':generate_password_hash(password),
			'real_name':real_name,
			'art_name':art_name,
			'role':role,
			'year_of_birth':year_of_birth,
			'country':country,
			'city':city,
			'email':email,
			'albums': [],
			'request_list': {}, # 요청받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
			'response_list': {}, # 요구받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
		}
		
		r.inc('the_number_of_artists')
		if r.set_data(artist_id, user):
			return redirect(url_for('login'))
		return render_template('500.html')
	return render_template('register.html', data=data)


@app.route('/login', methods=['GET', 'POST'])
def login():
	"""Login Form"""
	if request.method == 'GET':
		return render_template('login.html')
	else:
		artist_id = request.form['artist_id']
		password = request.form['password']
		if artist_id is None or password is None:
			return render_template('login.html')
		
		rs = RedisSession()
		user = rs.get_data(artist_id)
		
		msg = 'Invalid information.'
		if user is None:
			msg = 'The artist, {}, does not exist.'.format(artist_id)
		elif check_password_hash(user['password'], password):
			session['session_key'] = rs.save(artist_id)
			return redirect(url_for('home'))
	
	return render_template('login.html', msg=msg)
		

@app.route('/logout')
def logout():
	"""Logout Form"""
	if 'session_key' in session:
		del session['session_key']
	return redirect(url_for('home'))


@app.route('/about', methods=['GET', 'POST'])
def contact():
	r = RedisSession()
	user = check_session(r)

	if request.method == 'POST':
		sender = request.form['sender']
		content = request.form['content']
		attachment = request.files.get('attachment')
		return render_template('contact.html', msg='Complete. Thank You !', user=user)
	
	return render_template('contact.html', user=user)


@app.route('/profile/<artist_id>/<msg>', methods=['GET', 'POST'])
@app.route('/profile/<artist_id>', methods=['GET', 'POST'])
def profile(artist_id, msg=None):
	r = RedisSession()
	user = check_session(r)

	data = {}
	with open('dataset/user-role.txt', 'r') as f1, \
		open('dataset/country.txt', 'r') as f2, \
		open('dataset/city.txt', 'r') as f3:
		data['role'] = f1.read().split(', ')
		data['country'] = f2.read().split(', ')
		data['city'] = f3.read().split(', ')

	if request.method == 'POST':
		if user is None:
			return render_template('404.html')
		if user['artist_id'] != artist_id:
			return render_template('404.html')
		
		albums = {}
		collaborators = {}
		for album_id in user['albums']:
			album = r.get_data(album_id)
			albums[album_id] = album

			for _id in album['artist_id']:
				if _id not in collaborators.keys():
					artist = r.get_data(_id)
					name = artist['art_name'] if artist['art_name'] else artist['real_name']
					role = artist['role']
					collaborators[_id] = [role, [album_id]]
				else:
					collaborators[_id][1].append(album_id)
		
		tmp = [ _id for _id, value in user['request_list'].items() if filter_collaborators(value) ]
		tmp.extend([ _id for _id, value in user['response_list'].items() if filter_collaborators(value) ])
		for _id in tmp:
			if _id not in collaborators.keys():
				artist = r.get_data(_id)
				name = artist['art_name'] if artist['art_name'] else artist['real_name']
				role = artist['role']
				collaborators[_id] = [role, []]

		if user['artist_id'] in collaborators.keys():
			del collaborators[user['artist_id']]
		
		data['albums'] = albums
		data['collaboration'] = collaborators

		user['list'] = data
		current_password = request.form['password']
		if not check_password_hash(user['password'], current_password):
			return render_template('profile.html', data=user, user=user, msg='Invalid Password.')
		
		user['real_name'] = request.form['real_name']
		user['art_name'] = request.form['art_name']
		user['email'] = request.form['email']
		user['year_of_birth'] = request.form['year_of_birth']
		user['role'] = request.form['role']
		user['country'] = request.form['country']
		user['city'] = request.form['city']

		new_password = request.form['new_password']
		if len(new_password) > 0:
			if new_password != request.form['re_new_password']:
				return render_template('profile.html', data=user, user=user, msg='Invalid New Password.')
			user['password'] = generate_password_hash(new_password)
		
		del user['list']
		r.set_data(user['artist_id'], user)
		user['list'] = data
		return render_template('profile.html', data=user, user=user, msg='Saved.')

	other = user
	if user is None:
		other = r.get_data(artist_id)	
	elif user['artist_id'] != artist_id:
		other = r.get_data(artist_id)

	if other is None:
		return render_template('404.html')
	
	albums = {}
	collaborators = {}
	for album_id in other['albums']:
		album = r.get_data(album_id)
		albums[album_id] = album

		for _id in album['artist_id']:
			if _id not in collaborators.keys():
				artist = r.get_data(_id)
				name = artist['art_name'] if artist['art_name'] else artist['real_name']
				role = artist['role']
				collaborators[_id] = [role, [album_id]]
			else:
				collaborators[_id][1].append(album_id)
	
	tmp = [ _id for _id, value in other['request_list'].items() if filter_collaborators(value) ]
	tmp.extend([ _id for _id, value in other['response_list'].items() if filter_collaborators(value) ])
	for _id in tmp:
		if _id not in collaborators.keys():
			artist = r.get_data(_id)
			name = artist['art_name'] if artist['art_name'] else artist['real_name']
			role = artist['role']
			collaborators[_id] = [role, []]

	if other['artist_id'] in collaborators.keys():
		del collaborators[other['artist_id']]

	data['albums'] = albums
	data['collaboration'] = collaborators

	other['list'] = data
	return render_template('profile.html', data=other, user=user, msg=msg, show=1)


@app.route('/album/produce')
@app.route('/album/<int:album_id>/<int:status>')
def album(album_id=None, status=1):
	r = RedisSession()
	user = check_session(r)
	genre_list = None
	with open('dataset/album-genre.txt', 'r') as f:
		genre_list = f.read().split(', ')

	album = None
	if album_id is not None:
		album = r.get_data(album_id)
	if status == 0:
		return render_template('album.html', data=album, user=user, status=status) # status: 0=show

	if user is None:
		return render_template('404.html')
	
	collaborators = [ _id for _id, value in user['request_list'].items() if filter_collaborators(value) ]
	collaborators.extend([ _id for _id, value in user['response_list'].items() if filter_collaborators(value) ])
	
	if status == 1:
		collaborators = list(set(collaborators))
		return render_template('album.html', user=user, collaborators=collaborators, genre_list=genre_list, status=status) # form-page = produce
	
	collaborators = list(set(collaborators + album['artist_id'][1:]))
	return render_template('album.html', data=album, user=user, collaborators=collaborators, genre_list=genre_list, status=status) # status: 2:modify
	

@app.route('/album/save', methods=['GET', 'POST'])
@app.route('/album/save/<int:album_id>', methods=['GET', 'POST'])
def save_album(album_id=0):
	r = RedisSession()
	user = check_session(r)
	
	if user is None or request.method == 'GET':
		return render_template('404.html')

	artist_id = [user['artist_id']] + request.form['collaborators'].split()
	album_title = request.form['album_title']
	genre = request.form['genre']
	year_of_pub = request.form['year_of_pub']
	num_of_tracks = request.form['num_of_tracks']
	num_of_sales = request.form['num_of_sales']

	album_id = str(r.inc('the_number_of_albums') + MINIMUM_ALBUM_NUMBER) if album_id == 0 else str(album_id)
	album = {
		'album_id': album_id,
		'artist_id': artist_id,
		'album_title': album_title,
		'genre': genre,
		'year_of_pub': year_of_pub,
		'num_of_tracks': num_of_tracks if num_of_tracks else 0,
		'num_of_sales': num_of_sales if num_of_sales else 0,
	}

	r.set_data(album_id, album)
	for _id in artist_id:
		artist = r.get_data(_id)
		if album_id not in artist['albums']:
			artist['albums'].append(album_id)
			r.set_data(_id, artist)

	return redirect(url_for('album', album_id=album_id, status=0))


@app.route('/album/delete/<int:album_id>')
def delete(album_id):
	r = RedisSession()
	user = check_session(r)
	
	if user is None:
		return render_template('404.html')

	msg = "Error"
	album_id = str(album_id)
	album = r.get_data(album_id)
	for artist_id in album['artist_id']:
		if user['artist_id'] == artist_id:
			user['albums'].remove(album_id)
			r.set_data(artist_id, user)
		else:
			artist = r.get_data(artist_id)
			artist['albums'].remove(album_id)
			r.set_data(artist_id, artist)
	
	if r.delete(album_id):
		msg = "Deleted"
	else:
		return render_template('500.html')
	
	return redirect(url_for('profile', artist_id=user['artist_id'], msg=msg))
	

@app.route('/collaborate/<artist_id>')
def collaborate(artist_id):
	r = RedisSession()
	user = check_session(r)
	
	if user is None:
		return redirect(url_for('login'))
	
	_from = user['artist_id']
	_to = artist_id
	
	other = r.get_data(_to)
	now = datetime.now()
	_datetime = datetime.strftime(now, '%Y-%m-%d %H:%M:%S')
	
	if _from in other['response_list']:
		target = datetime.strptime(other['response_list'][_from][1], '%Y-%m-%d %H:%M:%S')
		diff = now - target
		if diff.days <= 1:
			return redirect(url_for('profile', artist_id=artist_id, msg='1 day 1 request'))

	user['request_list'][_to] = [0,_datetime]
	if not r.set_data(_from, user):
		return render_template('500.html')
	other['response_list'][_from] = [0,_datetime]
	if not r.set_data(_to, other):
		return render_template('500.html')

	return redirect(url_for('profile', artist_id=artist_id, msg='Success !'))


@app.route('/accept_collaboration/<artist_id>')
def accept_collaboration(artist_id):
	r = RedisSession()
	user = check_session(r)
	
	if user is None:
		return render_template('404.html')

	other = r.get_data(artist_id)
	other['request_list'][user['artist_id']][0] = 1
	r.set_data(artist_id, other)

	user['response_list'][artist_id][0] = 1
	r.set_data(user['artist_id'], user)

	return redirect(url_for('profile', artist_id=user['artist_id'], msg="Accepted"))
	

@app.route('/reject_collaboration/<artist_id>')
def reject_collaboration(artist_id):
	r = RedisSession()
	user = check_session(r)
	
	if user is None:
		return render_template('404.html')
	
	other = r.get_data(artist_id)
	other['request_list'][user['artist_id']][0] = 2
	r.set_data(artist_id, other)

	user['response_list'][artist_id][0] = 2
	r.set_data(user['artist_id'], user)
	return redirect(url_for('profile', artist_id=user['artist_id'], msg="Rejected"))


if __name__ == '__main__':
	app.run(host='0.0.0.0', port=80)
