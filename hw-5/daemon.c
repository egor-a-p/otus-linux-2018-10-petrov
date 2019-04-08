#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>

void signal_handler(int signalno){
	if (SIGTERM == signalno) {
		syslog(LOG_WARNING, "Catch SIGTERM");
		exit(0);
	}
}

int main(){
	// создаем потомка
	int pid = fork();

	if (!pid){ // если это потомок
		//Добавляем обработчик согнала SIGTERM
		signal(SIGTERM, signal_handler);

		// Демонизируем процесс:
        // данный код уже выполняется в процессе потомка
        // разрешаем выставлять все биты прав на создаваемые файлы, 
        // иначе у нас могут быть проблемы с правами доступа
        // создаём новый сеанс, чтобы не зависеть от родителя
        // переходим в корень диска, если мы этого не сделаем, то могут быть проблемы.
        // к примеру с размантированием дисков
        // закрываем дискрипторы ввода/вывода/ошибок, так как нам они больше не понадобятся
        umask(0);
        setsid();
        chdir("/");
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
		// Работа демона =)
		while(1){
			sleep(1);
		}

        return 0;
    } else {// если это родитель
		printf("Init daemon child with pid: %d\n", pid);
        // завершим процес, т.к. основную свою задачу (запуск демона) мы выполнили
        return 0;
    }
}

