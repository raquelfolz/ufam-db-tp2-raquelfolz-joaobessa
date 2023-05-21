FROM ubuntu:latest

RUN apt-get update && apt-get install -y build-essential

WORKDIR /app
COPY . /app

RUN g++ upload.cpp -o upload -Wall -O2 -static --std=c++17
RUN g++ findrec.cpp -o findrec -Wall -O2 -static --std=c++17
RUN g++ seek1.cpp -o seek2 -Wall -O2 -static --std=c++17
RUN g++ seek2.cpp -o seek2 -Wall -O2 -static --std=c++17
