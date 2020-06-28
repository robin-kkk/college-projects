from datetime import datetime
from redis import StrictRedis
from uuid import uuid4
import json
import time

to_dict = lambda x: dict(json.loads(x.decode('utf-8')))
to_json = lambda x: json.dumps(x, ensure_ascii=False).encode('utf-8')
MINIMUM_ALBUM_NUMBER = 2000000000

class RedisSession:
    prefix = 'was:session_key:'
    ip = '172.31.46.93'
    port = 6379
    timeout = 3600


    def __init__(self):
        self.db = StrictRedis(self.ip, self.port)


    def open(self, session_key):
        artist_id = self.db.get(self.prefix + session_key)
        if artist_id is not None:
            self.db.expire(self.prefix+session_key, self.timeout)
        else:
            return None
        return artist_id.decode()
    

    def save(self, artist_id):
        session_key = str(uuid4())
        self.db.setex(self.prefix+session_key, self.timeout, artist_id)
        return session_key


    def get_data(self, data_id):
        data = self.db.get(data_id)
        return to_dict(data) if data is not None else None
        

    def set_data(self, data_id, data):
        return self.db.set(data_id, to_json(data))
    

    def get(self, key):
        ret = self.db.get(key)
        return ret if ret is not None else 0

    
    def set(self, key, value):
        self.db.set(key, value)


    def inc(self, key):
        return self.db.incr(key)
    

    def dec(self, key):
        return self.db.decr(key)


    def delete(self, key):
        return self.db.delete(key)

    def keys(self):
        return [ key.decode() for key in self.db.keys() ]

    def drop(self):
        start = time.time()
        keys = self.keys()
        for key in keys:
            self.delete(key)
        end = time.time()
        print('[+] drop(): Duration {} sec'.format(end - start))

    def generate(self):
        import random
        import pandas as pd
        from datetime import datetime
        from werkzeug.security import generate_password_hash
        start = time.time()

        r = self
        albums_df = pd.read_csv("./dataset/albums.csv")
        artists_df = pd.read_csv("./dataset/artists.csv")
        r.set('the_number_of_albums', len(albums_df))
        r.set('the_number_of_artists', len(artists_df))

        alphabet = list('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_')
        random.shuffle(alphabet)
        numbers = list('0987654321')
        random.shuffle(numbers)

        id_check = {}
        for album in albums_df.values:
            id_len = random.randint(6,14)
            numbers_len = random.randint(0,int(id_len/2))
            
            new_artist_id = [ alphabet[random.randint(0,len(alphabet)-1)] for i in range(id_len-numbers_len) ]
            new_artist_id.extend([ numbers[random.randint(0,len(numbers)-1)] for i in range(numbers_len) ])
            new_artist_id = ''.join(new_artist_id)
            
            artist_id = album[1]
            album_id = str(album[0] + MINIMUM_ALBUM_NUMBER)

            if artist_id not in id_check.keys():
                artist = artists_df[artists_df.id == artist_id].values[0].tolist()

                artist_dict = {
                    'artist_id': new_artist_id,
                    'password': generate_password_hash(new_artist_id),
                    'real_name': artist[1],
                    'art_name': artist[2] if artist[2] else '',
                    'role': artist[3][0].upper() + artist[3][1:],
                    'year_of_birth': str(artist[4]),
                    'country': artist[5],
                    'city': artist[6],
                    'email': artist[7],
                    'albums': [album_id],
                    'request_list': {}, # 요청받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
                    'response_list': {}, # 요구받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
                }

                # Collaborators
                now = datetime.strftime(datetime.now(), '%Y-%m-%d %H:%M:%S')
                nums_collaborators = random.randint(0,12)
                new_id_check = list(id_check.values())
                if len(id_check) > nums_collaborators:
                    for i in range(nums_collaborators):
                        pos = random.randint(0, len(new_id_check)-1)
                        list_name = ['request_list', 'response_list']
                        data = [random.randint(0,2),now]
                        order = random.randint(0,1)
                        artist_dict[list_name[order]][new_id_check[pos]] = data
                        opponent = r.get_data(new_id_check[pos])
                        opponent[list_name[order^1]][new_artist_id] = data
                        r.set_data(opponent['artist_id'], opponent)

                r.set_data(new_artist_id, artist_dict)
                id_check[artist_id] = new_artist_id
            else:
                try:
                    new_artist_id = id_check[artist_id]
                    artist_dict = r.get_data(new_artist_id)
                    artist_dict['albums'].append(album_id)
                except Exception as e:
                    print(e)

            album_dict = {
                'album_id': album_id,
                'artist_id': [new_artist_id],
                'album_title': album[2],
                'genre': album[3],
                'year_of_pub': str(album[4]),
                'num_of_tracks': album[5],
                'num_of_sales': album[6],
            }
            
            r.set_data(album_id, album_dict)
        
        artists_having_album = set(albums_df.artist_id.values.tolist())
        all_artists = set(artists_df.id.values.tolist())
        diff = list(all_artists - artists_having_album)
        assert len(diff) == (len(all_artists) - len(id_check))
        
        for artist_id in diff:
            id_len = random.randint(6,14)
            numbers_len = random.randint(0,int(id_len/2))
            
            new_artist_id = [ alphabet[random.randint(0,len(alphabet)-1)] for i in range(id_len-numbers_len) ]
            new_artist_id.extend([ numbers[random.randint(0,len(numbers)-1)] for i in range(numbers_len) ])
            new_artist_id = ''.join(new_artist_id)

            artist = artists_df[artists_df.id == artist_id].values[0].tolist()
            
            artist_dict = {
                'artist_id': new_artist_id,
                'password': generate_password_hash(new_artist_id),
                'real_name': artist[1],
                'art_name': artist[2] if artist[2] else '',
                'role': artist[3][0].upper() + artist[3][1:],
                'year_of_birth': str(artist[4]),
                'country': artist[5],
                'city': artist[6],
                'email': artist[7],
                'albums': [album_id],
                'request_list': {}, # 요청받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
                'response_list': {}, # 요구받은 협업 목록 entry --> 'other_artist_id' : ['status (=0/1/2)', 'datetime']
            }

            r.set_data(new_artist_id, artist_dict)

        end = time.time()
        print("[+] generate(): Duration {} sec".format(end - start))
        


def filter_album_id(x):
    try:
    	y = int(x)
    	return True if y >= MINIMUM_ALBUM_NUMBER else False
    except:
    	return False


def filter_artist_id(x):
    if x.startswith('was:session_key'):
    	return False
    try:
    	int(x)
    	return False
    except:
    	return True


def filter_collaborators(arr):
    now = datetime.now()
    expire = datetime.strptime(arr[1], '%Y-%m-%d %H:%M:%S')
    diff = now - expire
    if arr[0] == 1 and diff.days <= 7:
    	return True
    return False

if __name__ == '__main__':
    RedisSession().generate()
