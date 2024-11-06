FROM ubuntu:latest

COPY cmake-build-debug/AxonClient /home/ubuntu
COPY cmake-build-debug/libAxonServerLib.so /usr/local/lib

RUN ldconfig

ENTRYPOINT ["sh", "-c", "./home/ubuntu/AxonClient"]

CMD ["run"]
