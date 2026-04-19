FROM ubuntu:24.04

# Non-interactive
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       ca-certificates \
       wget \
       make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source
COPY . /app

# Compile all .cpp files into `main` using g++ (explicit command avoids makefile quirks)
#RUN g++ -std=c++11 -Wall -ansi -pedantic -g *.cpp -o main -lm || (echo "Build failed" && exit 1)
RUN make

# Default working directory and entrypoint: run the compiled binary
ENTRYPOINT ["/app/main"]

# Default arguments: example run (can be overridden at `docker run` time)
CMD ["Original_instances/Random/R020306_0020_001.txt","11","1500","30","0.4","0.7","0.5","0.3","5","0.001","0.3","0.4","0.9"]
