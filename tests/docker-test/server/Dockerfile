FROM ubuntu:latest

COPY cmake-build-debug/AxonDockertestServer /home/ubuntu
COPY cmake-build-debug/libAxonServerLib.so /usr/local/lib

RUN ldconfig

ENTRYPOINT [ "./home/ubuntu/AxonDockertestServer" ]

CMD ["run"]
