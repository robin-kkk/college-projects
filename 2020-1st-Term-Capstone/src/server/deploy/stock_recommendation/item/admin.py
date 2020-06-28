from django.contrib import admin
from .models import StockItem, StockNews

# Register your models here.
class StockItemAdmin(admin.ModelAdmin):
    list_display = [ 'code', 'name', 'wics', 'now_val', 'fluctuation', 'diff', 'volume' ]
    list_display_links = [ 'code', 'name', 'wics', 'now_val', 'fluctuation', 'diff', 'volume' ]
    list_filter = ['wics']
    search_fields = ['code', 'name', 'wics']


class StockNewsAdmin(admin.ModelAdmin):
    list_display = [ 'item', 'keywords', 'news_count' ]
    list_display_links = [ 'item', 'keywords', 'news_count' ]
    search_fields = ['item', 'keywords']

admin.site.register(StockItem, StockItemAdmin)
admin.site.register(StockNews, StockNewsAdmin)

