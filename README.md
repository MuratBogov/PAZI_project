# ch_uid и cap_exec

Этот репозиторий содержит две программы на языке C — `ch_uid` и `cap_exec`, которые работают вместе для управления привилегиями и переключения UID в Linux.

## Обзор

### `ch_uid`
Эта программа создает дочерний процесс, меняет его UID и GID на указанные пользователя, и выводит UID и GID до и после изменения.

- **Основной функционал**:
  - Создает дочерний процесс с помощью `fork`.
  - Меняет UID и GID дочернего процесса на значения, соответствующие указанному пользователю.
  - Передает информацию об UID и GID обратно в родительский процесс через pipe.

### `cap_exec`
Эта программа предоставляет необходимые привилегии (`CAP_SETUID` и `CAP_SETGID`) для выполнения программы `ch_uid`, а затем сбрасывает их после выполнения.

- **Основной функционал**:
  - Устанавливает привилегии Linux: `CAP_SETUID` и `CAP_SETGID` для выполнения `ch_uid`.
  - Запускает `ch_uid` как дочерний процесс, передавая в него имя пользователя.
  - Сбрасывает привилегии после завершения работы дочернего процесса.

---

## Использование

### Компиляция
Скомпилируйте обе программы с помощью GCC:

`gcc -o ch_uid ch_uid.c`

`gcc -o cap_exec cap_exec.c`

Далее выдайте необходимые привилегии программе cap_exec:

`sudo setcap cap_setuid,cap_setgid+ep cap_exec`

### Запуск
Для выполнения программ используйте следующую команду:

`./cap_exec ./ch_uid <имя_пользователя>`

Где:
- <имя_пользователя> — имя пользователя, на которого будут изменены UID и GID.

### Пример
./cap_exec ./ch_uid john

Эта команда:
1. Использует cap_exec для предоставления привилегий CAP_SETUID и CAP_SETGID программе ch_uid.
2. Выполняет ch_uid, чтобы изменить UID и GID дочернего процесса на значения пользователя john.

---

## Файлы

1. `ch_uid.c`: Содержит код для создания дочернего процесса и смены его UID и GID на значения указанного пользователя.
2. `cap_exec.c`: Содержит код для управления привилегиями и безопасного выполнения программы ch_uid.

---
