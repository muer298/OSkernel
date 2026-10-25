# This is a local, illustrative build image, NOT an official judging image.
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu \
    qemu-system-misc make gcc coreutils ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
CMD ["make", "all"]
