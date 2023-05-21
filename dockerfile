FROM ubuntu:latest

RUN apt-get update && apt-get install -y build-essential

WORKDIR /app
COPY . /app

RUN g++ upload.cpp -o upload -Wall -O2 -static --std=c++17
RUN ./upload
