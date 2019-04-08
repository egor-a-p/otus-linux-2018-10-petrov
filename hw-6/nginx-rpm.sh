#!/usr/bin/env bash

cd ~

wget https://nginx.org/packages/centos/7/SRPMS/nginx-1.14.1-1.el7_4.ngx.src.rpm
wget https://www.openssl.org/source/latest.tar.gz
rpm -i nginx-1.14.1-1.el7_4.ngx.src.rpm