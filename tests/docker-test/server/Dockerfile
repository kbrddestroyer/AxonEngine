FROM ubuntu:latest

COPY cmake-build-debug/AxonServer /home/ubuntu
COPY cmake-build-debug/libAxonServerLib.so /usr/local/lib

RUN ldconfig

EXPOSE 10243
ENTRYPOINT [ "./home/ubuntu/AxonServer" ]

CMD ["run"]
