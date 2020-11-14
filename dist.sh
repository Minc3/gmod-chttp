#!/bin/bash -e

# Ensure the machines are up
vagrant up

# Empty dist folder
rm -rf dist

# Build the different configurations
vagrant ssh -c "sudo schroot --chroot steamrt_scout_i386 -- /vagrant/dist-build-single.sh linux Debug"
vagrant ssh -c "sudo schroot --chroot steamrt_scout_amd64 -- /vagrant/dist-build-single.sh linux64 Debug"
vagrant ssh -c "sudo schroot --chroot steamrt_scout_i386 -- /vagrant/dist-build-single.sh linux Release"
vagrant ssh -c "sudo schroot --chroot steamrt_scout_amd64 -- /vagrant/dist-build-single.sh linux64 Release"
vagrant ssh -c "/vagrant/dist-build-single.sh win32 Debug"
vagrant ssh -c "/vagrant/dist-build-single.sh win64 Debug"
vagrant ssh -c "/vagrant/dist-build-single.sh win32 Release"
vagrant ssh -c "/vagrant/dist-build-single.sh win64 Release"
