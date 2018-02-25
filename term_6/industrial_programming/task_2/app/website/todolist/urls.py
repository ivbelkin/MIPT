from django.urls import include, path
from . import views

app_name = "todolist"

urlpatterns = [
    path("", views.index, name="index"),
    path("add_task/", views.add_task, name="add_task"),
    path("del_task/", views.del_task, name="del_task"),
]