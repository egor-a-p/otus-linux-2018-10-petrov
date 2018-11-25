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

##  ДЗ 2:

- Разбираемся с ruby, дорабатываем Vagrantfile, чтобы появилась возможность конфигурировать диски:
    ```
        disks_number = 6
        disk_size = 100 #MB
    ```
- Поднимаем виртуалку, смотрим, что получилось:
    ```
        lsblk
        -----------------
        NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
        sda      8:0    0   40G  0 disk
        `-sda1   8:1    0   40G  0 part /
        sdb      8:16   0  100M  0 disk
        sdc      8:32   0  100M  0 disk
        sdd      8:48   0  100M  0 disk
        sde      8:64   0  100M  0 disk
        sdf      8:80   0  100M  0 disk
        sdg      8:96   0  100M  0 disk
    ```
- Будем собирать (-v или --verbose для подробного вывода) RAID 10 (-l или --level 10) на 6 дисках (-n или --raid-devices 6):
    ```
        sudo yum install -y mdadm smartmontools hdparm gdisk #Доустановим пакеты
        sudo mdadm --zero-superblock --force /dev/sd{b,c,d,e,f,g} #Занулим на всāкий слуùай суперблоки
        sudo mdadm --create --verbose /dev/md0 -l 10 -n 6 /dev/sd{b,c,d,e,f,g} #Соберем RAID
        -----------------
        mdadm: layout defaults to n2
        mdadm: layout defaults to n2
        mdadm: chunk size defaults to 512K
        mdadm: size set to 100352K
        mdadm: Defaulting to version 1.2 metadata
        mdadm: array /dev/md0 started.
    ```
- Проверим, что RAID собрался нормально:
    ```
        cat /proc/mdstat
        --------
        Personalities : [raid10]
        md0 : active raid10 sdg[5] sdf[4] sde[3] sdd[2] sdc[1] sdb[0]
              301056 blocks super 1.2 512K chunks 2 near-copies [6/6] [UUUUUU]

        unused devices: <none>
    ```
- Разберем RAID:
    ```
        sudo umount /dev/md0
        sudo mdadm -S /dev/md0
        sudo mdadm --zero-superblock --force /dev/sd{b,c,d,e,f,g}
    ```
- Напишем скрипт, который будет собирать данный RAID и перенесем его в виртуалку:
    ```
        scp -i .vagrant/machines/otuslinux/virtualbox/private_key init_raid.sh  vagrant@192.168.11.101:~
    ```
- И воспользуемся им:
    ```
        chmod +x init_raid.sh
        sudo ./init_raid.sh
    ```
- Сломаем и починим RAID:
    ```
        [vagrant@otuslinux ~]$ sudo mdadm /dev/md0 --fail /dev/sdg
        mdadm: set /dev/sdg faulty in /dev/md0
        [vagrant@otuslinux ~]$ sudo mdadm /dev/md0 --remove /dev/sdg
        mdadm: hot removed /dev/sdg from /dev/md0
        [vagrant@otuslinux ~]$ sudo mdadm /dev/md0 --add /dev/sdg
        mdadm: added /dev/sdg
    ```
- Создадим GPT раздел, две партиции и смонтируем их
    ```
        sudo parted -s /dev/md0 mklabel gpt
        sudo parted /dev/md0 mkpart primary xfs 0% 50%
        sudo parted /dev/md0 mkpart primary xfs 50% 100%
        for i in $(seq 1 2); do sudo mkfs.xfs -f /dev/md0p$i; done
        sudo mkdir -p /raid/part{1,2}
        for i in $(seq 1 2); do sudo mount /dev/md0p$i /raid/part$i; done
    ```
- Создадим файл mdadm.conf:
    ```
        sudo su
        mkdir /etc/mdadm
        echo "DEVICE partitions" > /etc/mdadm/mdadm.conf
        mdadm --detail --scan --verbose | awk '/ARRAY/ {print}' >> /etc/mdadm/mdadm.conf
        chmod 665 /etc/mdadm/mdadm.conf
    ```
- И скопируем:
    ```
        scp -i .vagrant/machines/otuslinux/virtualbox/private_key vagrant@192.168.11.101:/etc/mdadm/mdadm.conf ./
    ```
- Использую роль [jacoelho.softwareraid](https://github.com/jacoelho/ansible-softwareraid/blob/master/test_default.yml) запровиженим создание RAIDа