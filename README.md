# egor-a-p
otus linux 10-2018 repo

##  ДЗ 1:
- ключ лежит в .vagrant/machines/otuslinux/virtualbox/private_key
- ssh и scp запускать с -i .vagrant/machines/otuslinux/virtualbox/private_key
    ```
        ssh -i .vagrant/machines/otuslinux/virtualbox/private_key vagrant@192.168.11.101
    ```
- Смотрим, что есть в /boot
    ```
        ll /boot/
        --------------
        total 22336
        -rw-------. 1 root root  3414754 Sep 26 15:15 System.map-3.10.0-862.14.4.el7.x86_64
        -rw-r--r--. 1 root root   147859 Sep 26 15:15 config-3.10.0-862.14.4.el7.x86_64
        drwxr-xr-x. 3 root root     4096 Sep 30 19:58 efi
        drwx------. 5 root root     4096 Sep 30 20:02 grub2
        -rw-------. 1 root root 12583653 Sep 30 20:03 initramfs-3.10.0-862.14.4.el7.x86_64.img
        -rw-r--r--. 1 root root   305158 Sep 26 15:17 symvers-3.10.0-862.14.4.el7.x86_64.gz
        -rwxr-xr-x. 1 root root  6398144 Sep 26 15:15 vmlinuz-3.10.0-862.14.4.el7.x86_64
    ```
- Копируем к себе текущий конфиг
    ```
        scp -i .vagrant/machines/otuslinux/virtualbox/private_key vagrant@192.168.11.101:/boot/config* ./hw-1/
    ```
- Смотрим, какое сейчас ядро
    ```
        uname -r
        ------------
        3.10.0-862.14.4.el7.x86_64
    ```
- Скачиваем и распаковываем ядро
    ```
        curl -OL https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.19.2.tar.xz
        curl -OL https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.19.2.tar.sign
        unxz linux-4.19.2.tar.xz 
        tar -xf linux-4.19.2.tar
    ```
- Доустанавливаем пакеты (что-то нашел в инструкции, остальное уже добирал по error-ам), собираем ядро:
    ```
        sudo yum install ncurses-devel make gcc bc openssl-devel elfutils-libelf-devel rpm-build bison flex
        sudo chmod 665 /var/log/yum.log
        cd linux-4.19.2
        cp /boot/config* .config && make oldconfig && make && make install && make modules_install
    ```
- Копируем к себе yum.log (сначала пришлось поднять права 665) и результирующий конфиг
    ```
        scp -i .vagrant/machines/otuslinux/virtualbox/private_key vagrant@192.168.11.101:/var/log/yum.log ./hw-1/
        scp -i .vagrant/machines/otuslinux/virtualbox/private_key vagrant@192.168.11.101:~/linux-4.19.2/.config ./hw-1/
    ```