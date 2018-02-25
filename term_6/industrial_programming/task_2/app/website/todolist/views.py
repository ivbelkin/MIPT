from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect
from django.contrib.auth import authenticate, login
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User

from .models import Task

# Create your views here.


# Availiable lists
def index(request):
    context = {
        "tasks": Task.objects.all()
    }
    return render(request, "todolist/index.html", context)


def add_task(request):
    Task(task_name=request.POST["Task_name"]).save()
    return HttpResponseRedirect("/")


def del_task(request):
    Task.objects.filter(pk__in=request.POST.getlist("task_to_del")).delete()
    return HttpResponseRedirect("/")
