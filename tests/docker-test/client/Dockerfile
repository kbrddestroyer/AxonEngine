FROM ubuntu:latest

COPY cmake-build-debug/AxonDockertestClient /home/ubuntu
COPY cmake-build-debug/libAxonClientLib.so /usr/local/lib

RUN ldconfig

ENTRYPOINT ["sh", "-c", "./home/ubuntu/AxonDockertestClient"]

CMD ["run"]
