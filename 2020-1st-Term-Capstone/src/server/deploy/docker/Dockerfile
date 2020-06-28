FROM ubuntu:16.04

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y \
			build-essential checkinstall libreadline-gplv2-dev \
			libncursesw5-dev libssl-dev libsqlite3-dev tk-dev \
			libgdbm-dev libc6-dev libbz2-dev zlib1g-dev openssl \
			libffi-dev python3-dev python3-setuptools python3 python3-pip \
			wget vim git gcc language-pack-ko

# Install python 3.7.7

RUN mkdir /tmp/py377
WORKDIR /tmp/py377
RUN wget https://www.python.org/ftp/python/3.7.7/Python-3.7.7.tar.xz
RUN tar -xvf Python-3.7.7.tar.xz
WORKDIR /tmp/py377/Python-3.7.7
RUN ./configure
RUN make altinstall
RUN update-alternatives --install /usr/bin/python3 python /usr/local/bin/python3.7 10
RUN ln -s /usr/bin/python3 /usr/bin/python
RUN ln -s /usr/local/bin/pip3.7 /usr/bin/pip
RUN pip install --upgrade pip


# Install Khaiii

RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8
RUN pip install cmake
WORKDIR /tmp
RUN git clone https://github.com/ke2ek/khaiii.git
WORKDIR /tmp/khaiii
RUN mkdir build
WORKDIR /tmp/khaiii/build
RUN cmake ..
RUN make all
RUN make large_resource
RUN make install
RUN make package_python
WORKDIR /tmp/khaiii/build/package_python
RUN pip install .


# Install requirements

RUN mkdir /docker
RUN mkdir /code
WORKDIR /docker
COPY . .
RUN pip install -r requirements.txt
WORKDIR /code
