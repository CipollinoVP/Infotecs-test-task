# Тестовое задание

## Руководство по использованию библиотеки journal.h

Заголовочный файл в папке include, исходный код в src.

### Сборка библиотеки

Для получения бинарного файла библиотеки в папке репозитория выполните
```
make make_library
```

### Структура и доступные методы

Библиотека имеет специальный тип приоритетов t_priority. Приоритет может принимать значения Low, Medium, High.

Класс журнала имеет название "Journal" и при инициализации должен принимать адрес в виде std::string и приоритет по умолчанию типа t_priority.

Класс имеет два метода. Первый
```
void Journal::change_min_priority(t_priority min_priority);
```
меняет приоритет по умолчанию на min_priority.
Второй
```
void Journal::add_message(const std::string& message, t_priority priority);
```
добавляет сообщение message в журнал с приоритетом priority и текущим временем. Если приоритет ниже приоритета журнала по умолчанию, сообщение добавлено не будет.
Формат сообщения при вызове journal.add_message("My message", Low)
```
Mon Jan 01 00:00:00 1960 MSK Low My message
```

## Руководство по использованию тестового приложения

Исходный код целиком в main.cpp. Для сборки необходимо выполнить
```
make make_application
```
если библиотека уже собрана, или
```
make
```
для сборки всего проекта.
Для запуска приложения после сборки введите:
```
./test_application /Your/Path/To/Journal.txt 1
```
Здесь /Your/Path/To/Journal.txt - путь до журнала, 1 - приоритет по умолчанию.

Приоритеты вводятся следующим образом:
0 - Low
1 - Medium
2 - High

Если хотите ввести сообщение введите строку, начинающуюся не с '-', в этой же строке опционально последним символом укажите приоритет и перейдите на следующую строку.

Если хотите выйти из приложения введите '-exit'

Если хотите сменить приоритет по умолчанию введите '-change' и в новой строке приоритет.
Пример работы:
```
user@desktop:~/WorkDir$ ./test_application "/home/user/Journal.txt" 1
Message first 1
Message second 0
Message third 2
-change
0
Message last 0
exit
-exit
```
Файл /home/user/Journal.txt
```
Mon Jan 20 18:10:48 2025 MSK Medium Message first
Mon Jan 20 18:11:03 2025 MSK High Message third
Mon Jan 20 18:11:33 2025 MSK Low Message last
Mon Jan 20 18:11:36 2025 MSK Low exit
```