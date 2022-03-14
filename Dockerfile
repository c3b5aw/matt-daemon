FROM ubuntu:latest

RUN	apt-get update
RUN apt-get install build-essential clang netcat -y

CMD [ "/bin/bash" ]