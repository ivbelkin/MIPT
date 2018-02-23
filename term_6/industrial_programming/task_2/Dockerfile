FROM python:3.6

ENV PYTHONUNBUFFERED=1

WORKDIR /app

EXPOSE 8000

COPY requirements.txt .

RUN pip install --no-cache -r requirements.txt

CMD [ "/bin/bash" ]
