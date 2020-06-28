from django.db import models
from django.utils import timezone

'''
code : str (PK) 			; 종목 코드
name : str					; 기업 이름
wics : str					; 업종
now_val : int 				; 현재가
fluctuation : float			; 등락률
diff : int					; 전일비
volume : long int			; 거래량
'''
class StockItem(models.Model):
    code = models.CharField(max_length=12, primary_key=True)
    name = models.CharField(max_length=20, blank=True)
    wics = models.CharField(max_length=30, blank=True)
    now_val = models.PositiveIntegerField(default=0)
    diff = models.IntegerField(default=0)
    fluctuation = models.FloatField(default=0.0)
    volume = models.BigIntegerField(default=0)

    class Meta:
        ordering = ['code']

    def __str__(self):
        return '{}({})'.format(self.name, self.code)
    
    def convert_to_dict(self):
        d = self.__dict__
        del(d['_state'])
        return d

'''
code : str (PK)				; 종목 코드
keywords : str				; 뉴스 키워드 (최대 20개)
news_count : int			; 키워드 추출 시 가져온 뉴스 개수
most_recent_pub : datetime	; 마지막 뉴스 날짜
'''
class StockNews(models.Model):
    item = models.OneToOneField(StockItem, on_delete=models.CASCADE)
    keywords = models.TextField()
    news_count = models.PositiveIntegerField()
    most_recent_pub = models.DateTimeField(default=timezone.now())

    class Meta:
        ordering = ['item']
    
    def __str__(self):
        return self.item.__str__()

    def convert_to_dict(self):
        from datetime import datetime
        return {
            'code': self.item.code,
            'name': self.item.name,
            'keywords': self.keywords,
            'news_count': self.news_count,
            'most_recent_pub': datetime.strftime(self.most_recent_pub, '%Y-%m-%d %H:%M:%S')
        }

