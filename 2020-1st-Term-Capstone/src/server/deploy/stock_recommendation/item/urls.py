"""stock URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.urls import path
from . import views


urlpatterns = [
    path('main/rmd_by_hottest/', views.rmd_by_hottest),
    path('main/rmd_by_random/', views.rmd_by_random),
    path('main/get_hottest/', views.get_hottest),
    path('main/get_goldencross/', views.get_goldencross),
    path('main/rmd_by_item/', views.rmd_by_item),
    path('tag/rmd_by_default/', views.rmd_by_default),
    path('tag/rmd_by_tags/', views.rmd_by_tags),
]

