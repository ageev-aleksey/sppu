#ifndef SDDCLIENT_CONTOURHDDRIVER_H
#define SDDCLIENT_CONTOURHDDRIVER_H
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
// Считаем 3 МБайта максимальным с запасом размером JPEG кадра.
#define MAX_JPEG_SIZE (3*1024*1024)
#define RECV_BUFF_SIZE (MAX_JPEG_SIZE + 65536)
// UDP-порт ВК по-умолчанию.
#define UDPVIDEO_PORT (47511)
// По-умолчанию в ВК установлены логин-пароль "root" и "0000". Закодированная
// в кодировке Base64 строка "root:0000":
#define AUTH_ROOT0000 "cm9vdDowMDAw"

#define VKVR_STATE_UNINITIALIZED (0)
#define VKVR_STATE_STOPPED       (1)
#define VKVR_STATE_CAPTURING     (2)
#define VKVR_STATE_WSA_ERROR     (3)

#define VKVR_OK                  (0)
#define VKVR_ERR_BAD_ADAPTER     (1)
#define VKVR_ERR_BAD_ALLOC       (2)
#define VKVR_ERR_BAD_THREAD      (3)
#define VKVR_ERR_CAP_FAILED      (4)
#define VKVR_ERR_NO_ADAPTERS     (5)
#define VKVR_ERR_NO_CAMERAS      (6)
#define VKVR_ERR_NO_WINPCAP      (7)
#define VKVR_ERR_UNINITIALIZED   (8)
#define VKVR_ERR_WRONG_STATE     (9)
#define VKVR_ERR_BAD_PARAMS     (10)
#define VKVR_ERR_NET_FAILED     (11)
#define VKVR_ERR_NO_CONNECT     (12)
#define VKVR_ERR_UNKNOWN        (13)
#define VKVR_ERR_SEND_FAILED    (14)
#define VKVR_ERR_RECV_FAILED    (15)
#ifdef WIN32
//-----------------------------------------------------------------------------
// Copyright © 2012 ОАО Ижевский Радиозавод, http://www.irz.ru
//-----------------------------------------------------------------------------
// Класс захвата кадров с камеры.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_DEPRECATE
//-----------------------------------------------------------------------------
#include <Winsock2.h>
#include <windows.h>
#include <stdio.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Класс захвата кадров с камеры.
//-----------------------------------------------------------------------------
class VKVRCapture
{
public:

   // Тип функции обратного вызова.
   //    param     указатель переданный в функцию start()
   //    jpeg_data указатель на данные захваченного JPEG кадра
   //    jpeg_size размер захваченного JPEG кадра
   //    reason    VKVR_OK для захваченного кадра, иначе код ошибки
   typedef void CapCallback(void* param,
      const void* jpeg_data, unsigned int jpeg_size, int reason);

   // Инициализация класса (без поиска и подключения камеры).
   //    remote_port1 UDP-порт камеры, с которого будут передаваться
   //                 видео данные 1-го канала,
   //    remote_port2 UDP-порт камеры, с которого будут передаваться
   //                 видео данные 2-го канала.
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int init(unsigned short remote_port1 = UDPVIDEO_PORT,
            unsigned short remote_port2 = UDPVIDEO_PORT + 1);
   // Конфигурирование ("пустой" метод).
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int config();
   // Запуск захвата кадров во внутреннем потоке. Если функция обратного
   // вызова не задана, то захват не запустится, будет выполнен только
   // поиск камеры (можно использовать для проверки связи с камерой).
   //    callback функция обратного вызова для передачи кадра,
   //    param    указатель для передачи данных в функцию обратного вызова,
   //    channel  номер канала, с которого будет передаваться видео (1 или 2),
   //    ipaddr   IP адрес ВК в строковом виде ("X.X.X.X"),
   //    auth     закодированная в Base64 строка авторизации, в том
   //             виде, в котором она должна быть в пртоколе HTTP.
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int play(CapCallback* callback, void* param,
      int channel, const char* ipaddr, const char* auth = NULL);
   // Останов захвата с камеры. Может вызываться как в функции обратного
   // вызова так и из другого потока приложения.
   void stop();
   // Деинициализация класса (останов захвата, освобождение ресурсов).
   void free() throw();
   // Возвращает текущее состояние класса.
   int state() const;

private:

   int _state; // состояние объекта
   bool _terminate; // флаг завершения захвата кадров
   int _channel; // канал камеры 1 - 1-й, 2 - 2-й
   unsigned long _ipaddr; // IP адрес ВК для структуры IN_ADDR
   char _host[16]; // IP адрес ВК в строковом виде ("X.X.X.X")
   char _auth[34]; // закодированная в Base64 строка авторизации, в том
                   // виде, в котором она должна быть в пртоколе HTTP
   CapCallback* _callback; // функция обратного вызова
   void* _param; // параметр для передачи в функцию обратного вызова
   unsigned __int8* _jpeg_data; // данные JPEG кадра
   unsigned int _jpeg_size; // размер JPEG кадра
   int _max_msg_size;
   unsigned short _remote_port1; // UDP-порт ВК (назначается пользователем)
                                 // для 1-го канала
   unsigned short _remote_port2; // UDP-порт ВК (назначается пользователем)
                                 // для 2-го канала
   unsigned short _local_port; // UDP-порт ПЭВМ (выделяется ОС)
   SOCKET _udp_socket; // UDP-сокет для приёма видео с ВК
   SOCKET _http_socket; // TCP-сокет для посылки команд управления ВК
   HANDLE _work_thread; // поток захвата кадров
   DWORD _thread_id; // ID потока захвата кадров
   CRITICAL_SECTION _cs; // для синхронизации обращений к полям класса

   // Функция посылки HTTP запроса на запуск/останов передачи видео
   // по сети.
   //    cmd строковая команда: "play" или "stop".
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int http_request(char* cmd);
   // Функция ищет заголовок кадра и, если находит, сдвигает его в начало
   // буфера приёма данных.
   void find_begin();

public:

   // Конструктор.
   VKVRCapture();
   // Деструктор.
   ~VKVRCapture();

   // Вызов функции обратного вызова с кодом ошибки.
   void error_callback(int e);
   // Функция потока захвата кадров.
   bool capture_work_routine();
   // Функция запуска потока захвата кадров.
  // friend DWORD WINAPI run_capture_work_routine(LPVOID lpParameter);
};
//-----------------------------------------------------------------------------
#endif

#ifdef __linux__
class VKVRCapture
{
public:

   // Тип функции обратного вызова.
   //    param     указатель переданный в функцию start()
   //    jpeg_data указатель на данные захваченного JPEG кадра
   //    jpeg_size размер захваченного JPEG кадра
   //    reason    VKVR_OK для захваченного кадра, иначе код ошибки
   typedef void CapCallback(void* param,
      const void* jpeg_data, unsigned int jpeg_size, int reason);

   // Инициализация класса (без поиска и подключения камеры).
   //    remote_port1 UDP-порт камеры, с которого будут передаваться
   //                 видео данные 1-го канала,
   //    remote_port2 UDP-порт камеры, с которого будут передаваться
   //                 видео данные 2-го канала.
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int init(unsigned short remote_port1 = UDPVIDEO_PORT,
            unsigned short remote_port2 = UDPVIDEO_PORT + 1);
   // Конфигурирование ("пустой" метод).
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int config();
   // Запуск захвата кадров во внутреннем потоке. Если функция обратного
   // вызова не задана, то захват не запустится, будет выполнен только
   // поиск камеры (можно использовать для проверки связи с камерой).
   //    callback функция обратного вызова для передачи кадра,
   //    param    указатель для передачи данных в функцию обратного вызова,
   //    channel  номер канала, с которого будет передаваться видео (1 или 2),
   //    ipaddr   IP адрес ВК в строковом виде ("X.X.X.X"),
   //    auth     закодированная в Base64 строка авторизации, в том
   //             виде, в котором она должна быть в пртоколе HTTP.
   // Возвращает VKVR_OK в случае успеха, иначе код ошибки.
   int play(CapCallback* callback, void* param,
      int channel, const char* ipaddr, const char* auth = nullptr);
   // Останов захвата с камеры. Может вызываться как в функции обратного
   // вызова так и из другого потока приложения.
   void stop();
   // Деинициализация класса (останов захвата, освобождение ресурсов).
   void free() throw();
   // Возвращает текущее состояние класса.
   int state() const;


   VKVRCapture();
   // Деструктор.
   ~VKVRCapture();

   // Вызов функции обратного вызова с кодом ошибки.
   void error_callback(int e);
   // Функция потока захвата кадров.
   bool capture_work_routine();
   // Функция запуска потока захвата кадров.
  // friend DWORD WINAPI run_capture_work_routine(LPVOID lpParameter);
};
#endif

#endif