FROM ubuntu:latest

RUN apt-get update
RUN apt-get install build-essential clang netcat net-tools -y

WORKDIR /usr/matt_daemon

COPY Makefile .
COPY src src

RUN make

CMD [ "/bin/bash" ]