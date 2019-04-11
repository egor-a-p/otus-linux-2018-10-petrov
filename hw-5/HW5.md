##  ДЗ 5:

Демон с обработчиком сигналов.

 - соберем (нужен gcc):

```
[root@cs-ws174225 hw-5]# make
gcc daemon.c -fPIC -o daemon
```
 - запустим:

```
[root@cs-ws174225 hw-5]# ./daemon 
Init daemon child with pid: 20905
```

 - отправим SGTERM:

```
[root@cs-ws174225 hw-5]# kill 20905
```

 - проверим, как сработал обработчик:

```
[root@cs-ws174225 hw-5]# tail /var/log/messages
Apr  8 11:16:31 cs-ws174225 dbus[5797]: [system] Activating via systemd: service name='net.reactivated.Fprint' unit='fprintd.service'
Apr  8 11:16:31 cs-ws174225 systemd: Starting Fingerprint Authentication Daemon...
Apr  8 11:16:31 cs-ws174225 dbus[5797]: [system] Successfully activated service 'net.reactivated.Fprint'
Apr  8 11:16:31 cs-ws174225 systemd: Started Fingerprint Authentication Daemon.
Apr  8 11:16:35 cs-ws174225 su: (to root) e.petrov on pts/1
Apr  8 11:17:07 cs-ws174225 dbus[5797]: [system] Activating via systemd: service name='net.reactivated.Fprint' unit='fprintd.service'
Apr  8 11:17:07 cs-ws174225 systemd: Starting Fingerprint Authentication Daemon...
Apr  8 11:17:07 cs-ws174225 dbus[5797]: [system] Successfully activated service 'net.reactivated.Fprint'
Apr  8 11:17:07 cs-ws174225 systemd: Started Fingerprint Authentication Daemon.
Apr  8 11:17:24 cs-ws174225 daemon: Catch SIGTERM
```

