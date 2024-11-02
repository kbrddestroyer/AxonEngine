FROM ubuntu:latest

# Todo: replace local binary file w/ path to build target
COPY cmake-build /home/ubuntu

ENTRYPOINT ["sh", "-c", "sleep 5 && ./home/ubuntu/AxonClient"]
CMD ["run"]
