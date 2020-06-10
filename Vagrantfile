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

    # Install custom CMake
    apt purge --auto-remove cmake
    apt -y install libssl-dev
    CMAKE_VER=3.17
    CMAKE_BUILD=3
    mkdir /tmp/cmake-build
    cd /tmp/cmake-build
    wget -q https://cmake.org/files/v${CMAKE_VER}/cmake-${CMAKE_VER}.${CMAKE_BUILD}.tar.gz
    tar -xzf cmake-${CMAKE_VER}.${CMAKE_BUILD}.tar.gz
    cd cmake-${CMAKE_VER}.${CMAKE_BUILD}/
    ./bootstrap
    make -j4
    make install
    cd /
    rm -rf /tmp/cmake-build
  SHELL
end
