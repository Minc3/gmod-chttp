#!/bin/bash -e

export DEBIAN_FRONTEND=noninteractive

# HACK: Manually tell debian where to install grub
debconf-set-selections <<< "grub-pc grub-pc/install_devices string /dev/vda"

apt update && apt -y upgrade

apt -y install \
	build-essential \
	cmake \
	mingw-w64 \
	schroot \
	wget \

# Share some mounts
echo "/vagrant		/vagrant	none	rw,bind		0	0" >> /etc/schroot/default/fstab

cd ~

git clone --depth=1 https://github.com/ValveSoftware/steam-runtime

wget -q https://repo.steampowered.com/steamrt-images-scout/snapshots/latest-steam-client-general-availability/com.valvesoftware.SteamRuntime.Sdk-amd64,i386-scout-sysroot.tar.gz
wget -q https://repo.steampowered.com/steamrt-images-scout/snapshots/latest-steam-client-general-availability/com.valvesoftware.SteamRuntime.Sdk-i386-scout-sysroot.tar.gz

cd steam-runtime

./setup_chroot.sh --amd64 --tarball ../com.valvesoftware.SteamRuntime.Sdk-amd64,i386-scout-sysroot.tar.gz
./setup_chroot.sh --i386 --tarball ../com.valvesoftware.SteamRuntime.Sdk-i386-scout-sysroot.tar.gz
