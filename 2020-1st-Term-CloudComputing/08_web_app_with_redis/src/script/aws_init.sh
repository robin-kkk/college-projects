#!/bin/sh

# Install Required Packages
sudo apt-get -y update && sudo apt-get -y upgrade &&
	sudo apt-get install -y \
	make cmake build-essential libssl-dev zlib1g-dev libbz2-dev libreadline-dev \
	libsqlite3-dev libffi-dev libc6-dev libncurses5-dev libncursesw5-dev \
	wget curl llvm xz-utils tk-dev git tmux vim python python-dev python3-dev ;

# Install Python Virtual Environment And Docker (compose, machine) for Swarm
git clone https://github.com/pyenv/pyenv.git ~/.pyenv &&
	echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bash_profile &&
	echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bash_profile &&
	echo 'eval "$(pyenv init -)"' >> ~/.bash_profile &&
	git clone https://github.com/yyuu/pyenv-virtualenv.git ~/.pyenv/plugins/pyenv-virtualenv &&
	echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.bash_profile &&
	sh -c ". ~/.bash_profile" &&
	curl -fsSL https://get.docker.com/ | sudo sh &&
	curl -L "https://github.com/docker/compose/releases/download/1.22.0/docker-compose-$(uname -s)-$(uname -m)"  -o /usr/local/bin/docker-compose &&
	chmod +x /usr/local/bin/docker-compose &&
	base=https://github.com/docker/machine/releases/download/v0.16.0 &&
	curl -L $base/docker-machine-$(uname -s)-$(uname -m) >/tmp/docker-machine &&
	sudo install /tmp/docker-machine /usr/local/bin/docker-machine &&
	docker version && docker-compose version && docker-machine version ;

# Install Vim
git clone https://github.com/ke2ek/my_vim.git &&
	git clone https://github.com/VundleVim/Vundle.vim.git my_vim/.vim/bundle/Vundle.vim &&
	mv ./my_vim/.vim ~/ && mv ./my_vim/.vimrc ~/ && rm -dR ./my_vim &&
	vim -c "PluginInstall" -c q -c q! &&
	python ~/.vim/bundle/YouCompleteMe/install.py && # <--- failed here
	echo "source ~/.bash_profile" >> ~/.bashrc ;
