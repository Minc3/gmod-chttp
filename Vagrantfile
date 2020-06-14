# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.define "x64" do |x64|
    x64.vm.box = "generic/debian8"
  end

  config.vm.define "x86" do |x86|
    x86.vm.box = "timschumi/debian8-x32"
  end

  config.vm.synced_folder ".", "/vagrant", nfs_version: 4

  config.vm.provision "shell", inline: <<-SHELL
    export DEBIAN_FRONTEND=noninteractive
    apt update && apt -y upgrade
    apt -y install build-essential

    CMAKE_RELEASE="3.17.3_1"
    if [ "$(uname -m)" = "x86_64" ]; then
        CMAKE_ARCHIVE="cmake-linux-x64.tar.gz"
    else
        CMAKE_ARCHIVE="cmake-linux-x86.tar.gz"
    fi

    apt -y purge --auto-remove cmake
    wget -q "https://github.com/timschumi/cmake-static/releases/download/${CMAKE_RELEASE}/${CMAKE_ARCHIVE}"
    tar -xf "${CMAKE_ARCHIVE}" -C /usr/local
    rm -rf "${CMAKE_ARCHIVE}"
  SHELL
end
