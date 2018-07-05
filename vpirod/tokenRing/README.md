# TokenRing

## Запуск token ring
    go run tokenRing.go 5 1000   

go run tokenRing.go N timeout   

## Запуск управляющей программы
    go run maintenance.go   

Type - тип сообщения (send, drop, exit)   
Dist - куда будет отправлено сообщение при 'send'   
Data - текст сообщения   
Where - на какой узел отправить   
exit - выходит из управляющей программы

### Пример (запуск на двух консолях)
Первая консоль   

    go run tokenRing.go 5 1000   

Вторая консоль

    go run maintenance.go   
    <Type> <Dist> <Data> <Where> or 'exit'
    send 1 lol 2
    exit 1 kek 0
    exit
