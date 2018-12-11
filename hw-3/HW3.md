##  ДЗ 3:

- Забираем Vagrantfile
```
cd hw-3/
curl https://gitlab.com/otus_linux/stands/raw/master/LVM/Vagrantfile -o Vagrantfile
```
- Посмотрим блочные устройства на виртуалке
```
[vagrant@otuslinux ~]$ lsblk
NAME                    MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda                       8:0    0   40G  0 disk
|-sda1                    8:1    0    1M  0 part
|-sda2                    8:2    0    1G  0 part /boot
`-sda3                    8:3    0   39G  0 part
  |-VolGroup00-LogVol00 253:0    0 37.5G  0 lvm  /
  `-VolGroup00-LogVol01 253:1    0  1.5G  0 lvm  [SWAP]
sdb                       8:16   0   10G  0 disk
sdc                       8:32   0    2G  0 disk
sdd                       8:48   0    1G  0 disk
sde                       8:64   0    1G  0 disk
sdf                       8:80   0  256M  0 disk
sdg                       8:96   0  256M  0 disk
[vagrant@otuslinux ~]$ sudo lvmdiskscan
  /dev/VolGroup00/LogVol00 [     <37.47 GiB]
  /dev/VolGroup00/LogVol01 [       1.50 GiB]
  /dev/sda2                [       1.00 GiB]
  /dev/sda3                [     <39.00 GiB] LVM physical volume
  /dev/sdb                 [      10.00 GiB]
  /dev/sdc                 [       2.00 GiB]
  /dev/sdd                 [       1.00 GiB]
  /dev/sde                 [       1.00 GiB]
  /dev/sdf                 [     256.00 MiB]
  /dev/sdg                 [     256.00 MiB]
  6 disks
  3 partitions
  0 LVM physical volume whole disks
  1 LVM physical volume
```
- Начнем логгировать дз можно с помощью утилиты script
```
[root@otuslinux ~]# script hw-3.log
Script started, file is hw-3.log
```


### Из методички
- Создадим PV, VG, LG
```
[vagrant@otuslinux ~]$ sudo pvcreate /dev/sdb
  Physical volume "/dev/sdb" successfully created.
[vagrant@otuslinux ~]$ sudo vgcreate otus /dev/sdb
  Volume group "otus" successfully created
[vagrant@otuslinux ~]$ sudo  lvcreate -l+80%FREE -n test otus
  Logical volume "test" created.
```
- Посмотретим информацию о Volume Group
```
[vagrant@otuslinux ~]$ sudo vgdisplay -v otus
  --- Volume group ---
  VG Name               otus
  System ID
  Format                lvm2
  Metadata Areas        1
  Metadata Sequence No  2
  VG Access             read/write
  VG Status             resizable
  MAX LV                0
  Cur LV                1
  Open LV               0
  Max PV                0
  Cur PV                1
  Act PV                1
  VG Size               <10.00 GiB
  PE Size               4.00 MiB
  Total PE              2559
  Alloc PE / Size       2047 / <8.00 GiB
  Free  PE / Size       512 / 2.00 GiB
  VG UUID               qrO7hQ-QI9k-YqVq-mIx3-Uytu-j509-hrK5e4

  --- Logical volume ---
  LV Path                /dev/otus/test
  LV Name                test
  VG Name                otus
  LV UUID                V0VpWI-ZjdH-6hH0-jfgi-oLw5-f5Sa-X5PHo7
  LV Write Access        read/write
  LV Creation host, time otuslinux, 2018-11-25 13:08:29 +0000
  LV Status              available
  # open                 0
  LV Size                <8.00 GiB
  Current LE             2047
  Segments               1
  Allocation             inherit
  Read ahead sectors     auto
  - currently set to     8192
  Block device           253:2

  --- Physical volumes ---
  PV Name               /dev/sdb
  PV UUID               qDnWMg-l1Ln-fVX5-o1Hb-Fe0q-E8my-FZyH4K
  PV Status             allocatable
  Total PE / Free PE    2559 / 512
```
- Посмотрим информацию об LV:
```
[vagrant@otuslinux ~]$ sudo lvdisplay /dev/otus/test
  --- Logical volume ---
  LV Path                /dev/otus/test
  LV Name                test
  VG Name                otus
  LV UUID                V0VpWI-ZjdH-6hH0-jfgi-oLw5-f5Sa-X5PHo7
  LV Write Access        read/write
  LV Creation host, time otuslinux, 2018-11-25 13:08:29 +0000
  LV Status              available
  # open                 0
  LV Size                <8.00 GiB
  Current LE             2047
  Segments               1
  Allocation             inherit
  Read ahead sectors     auto
  - currently set to     8192
  Block device           253:2
```
- Информация в сжатом виде:
```
[vagrant@otuslinux ~]$ sudo vgs && sudo lvs
  VG         #PV #LV #SN Attr   VSize   VFree
  VolGroup00   1   2   0 wz--n- <38.97g    0
  otus         1   1   0 wz--n- <10.00g 2.00g
  LV       VG         Attr       LSize   Pool Origin Data%  Meta%  Move Log Cpy%Sync Convert
  LogVol00 VolGroup00 -wi-ao---- <37.47g
  LogVol01 VolGroup00 -wi-ao----   1.50g
  test     otus       -wi-a-----  <8.00g
```