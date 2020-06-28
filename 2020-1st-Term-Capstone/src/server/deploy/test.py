import requests
import time
import json

menu = '\nSelect api call\n \
[1] main/rmd_by_hottest\n \
[2] main/rmd_by_random\n \
[3] main/get_hottest\n \
[4] main/get_goldencross\n \
[5] main/rmd_by_item\n \
[6] tag/rmd_by_default\n \
[7] tag/rmd_by_tags\n\n \
(0 is quit) >> '

url = "http://localhost/"
headers = {'content-type': 'application/json'}
calls = ['', 'main/rmd_by_hottest/', 'main/rmd_by_random/', 'main/get_hottest/', 'main/get_goldencross/', 'main/rmd_by_item/', 'tag/rmd_by_default/', 'tag/rmd_by_tags/']

while True:
    try:
        choice = int(input(menu))
        if choice == 0:
            break
        s = time.time()
        if choice == 5:
            payload = {'code': '005930'}
            r = requests.post(url + calls[choice], data=json.dumps(payload), headers=headers)
        elif choice == 7:
            payload = {'tags': '삼성전자 특징주 골든크로스'}
            r = requests.post(url + calls[choice], data=json.dumps(payload), headers=headers)
        else:
            r = requests.get(url + calls[choice], headers=headers)
        print(r.status_code)
        print(r.text)
        e = time.time()
        print("Duration: {} sec".format(e - s))
    except Exception as e:
        print(e)



