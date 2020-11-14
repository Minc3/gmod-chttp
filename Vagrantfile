# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "generic/debian10"
  config.vm.provision "shell", path: "vagrant-provision.sh"
  config.vm.synced_folder ".", "/vagrant", nfs_version: 4
end
