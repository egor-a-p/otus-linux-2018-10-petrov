#!/usr/bin/env bash

declare -r LOCKFILE="/var/lock/watchdog.lock"
declare -r services=("sshd" "cups")

exclusive() {
    ( flock -n 9 || { echo "Watchdog is already running!"; exit 0; }
        trap 'rm -f $LOCKFILE' EXIT 
        "$@"
    ) 9>$LOCKFILE;
}

check() {
    array=($@)
    if [ -n "$array" ]; then
        for service in ${array[@]}; do # systemctl is-active --quiet $service || systemctl restart $service
            if systemctl list-unit-files --type service | awk "/^$service/ {print \$1}" | grep -qi "$service.service"; then
                state=$(systemctl show -p ActiveState ${service} | sed 's/ActiveState=//g')
                if [ "$state" != "active" ]; then
                        systemctl restart $service
                fi
            fi
        done
    fi
}

exclusive check "${services[@]}"

