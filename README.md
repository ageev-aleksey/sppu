# Клиент для взаимодействия с СППУ

### Возможности:
- Управление сферическим приводом прямого управления
- Построение графиков состояния устройства в реальном времени
- Запись данных о сотоянии устройства в json  файл (формат массив состояний)
- Возможность запуска программы без наличие устройства -
заменив устройство компьютерной моделью - утилита model_to_comport 
(необходима виртуальная пара последовтельных портов)

## Требуемые библиотеки

- Qt5.15 (Core, Widgets, SerialPort, PrintSupport)
(Необходимо указать путь до Qt5Find.cmake в корневом CMakeLists.txt данного проекта)
- Boost::Containers (Используется Boost.CircularBuffer)
- OpenCV 4.5.1
- libjpeg-turbo
- curl (libcul3-nss)
- jsoncpp 1.9.4
- matlab r2021b

Перед запуском matlab необходимо указать путь к библиотеки jsoncpp:
```shell script
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/
```
## Создание виртуальной пары последовательных портов в Ubuntu

```commandline
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

## Параметры последовательного порта

- BaudRate 38400
- ParityControl Disable
- FlowControl Disable
- ByteSize 8
- StopBit 1

# TODO
