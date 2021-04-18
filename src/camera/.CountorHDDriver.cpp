//#include "camera/CountourHdDriver.h"
//
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//
//
//
////-----------------------------------------------------------------------------
//// Copyright © 2012 ОАО Ижевский Радиозавод, http://www.irz.ru
////-----------------------------------------------------------------------------
//// Класс захвата кадров с камеры.
////-----------------------------------------------------------------------------
//#pragma hdrstop
//#pragma package(smart_init)
////-----------------------------------------------------------------------------
//#define MFOURCC(C1, C2, C3, C4) \
//   ( (unsigned __int32)((unsigned __int8)(C1)) + \
//    ((unsigned __int32)((unsigned __int8)(C2)) << 8) + \
//    ((unsigned __int32)((unsigned __int8)(C3)) << 16) + \
//    ((unsigned __int32)((unsigned __int8)(C4)) << 24) )
////-----------------------------------------------------------------------------
//// Объект синхронизации.
//class AutoLock
//{
//public:
//   AutoLock(LPCRITICAL_SECTION lpcs);
//   ~AutoLock();
//private:
//   LPCRITICAL_SECTION _lpcs;
//};
//AutoLock::AutoLock(LPCRITICAL_SECTION lpcs) :
//   _lpcs(lpcs)
//{
//   EnterCriticalSection(_lpcs);
//};
//AutoLock::~AutoLock()
//{ try {
//   LeaveCriticalSection(_lpcs);
//} catch (...) { } };
////-----------------------------------------------------------------------------
//// Конструктор.
//VKVRCapture::VKVRCapture() :
//   _state(VKVR_STATE_UNINITIALIZED),
//   _terminate(true),
//   _channel(1),
//   _ipaddr(0),
//   _callback(NULL),
//   _param(NULL),
//   _jpeg_data(NULL),
//   _jpeg_size(0),
//   _remote_port1(UDPVIDEO_PORT),
//   _remote_port2(UDPVIDEO_PORT + 1),
//   _local_port(0),
//   _udp_socket(INVALID_SOCKET),
//   _http_socket(INVALID_SOCKET),
//   _work_thread(NULL),
//   _thread_id(0)
//{
//   _host[0] = 0;
//   _auth[0] = 0;
//   // Инициализация объекта синхронизации critical section.
//   InitializeCriticalSection(&_cs);
//   // Подключение библиотеки WS2_32.DLL для использования сети.
//   WORD wVersionRequested = MAKEWORD(2, 2);
//   WSADATA wsaData;
//   if (WSAStartup(wVersionRequested, &wsaData) != 0)
//   {
//      _state = VKVR_STATE_WSA_ERROR;
//      return;
//   }
//   else if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
//   {
//      // Выгрузка библиотеки WS2_32.DLL - не та версия.
//      WSACleanup();
//      _state = VKVR_STATE_WSA_ERROR;
//      return;
//   };
//};
////-----------------------------------------------------------------------------
//// Деструктор.
//VKVRCapture::~VKVRCapture()
//{ try {
//   // Освобождение ресурсов, используемых объектом.
//   free();
//   // Освобождение ресурсов, используемых объектом critical section.
//   DeleteCriticalSection(&_cs);
//   // Выгрузка библиотеки WS2_32.DLL.
//   if (_state != VKVR_STATE_WSA_ERROR)
//   {
//      WSACleanup();
//   };
//} catch (...) { } };
////-----------------------------------------------------------------------------
//// Инициализация класса (без поиска и подключения камеры).
////    remote_port1 UDP-порт камеры, с которого будут передаваться
////                 видео данные 1-го канала,
////    remote_port2 UDP-порт камеры, с которого будут передаваться
////                 видео данные 2-го канала.
//// Возвращает VKVR_OK в случае успеха, иначе код ошибки.
//int VKVRCapture::init(unsigned short remote_port1, unsigned short remote_port2)
//{
//   AutoLock lock(&_cs);
//   if (_state == VKVR_STATE_UNINITIALIZED)
//   {
//      if (_jpeg_data == NULL)
//      {
//         _jpeg_data = new unsigned __int8[RECV_BUFF_SIZE];
//         if (_jpeg_data == NULL) return VKVR_ERR_BAD_ALLOC;
//      };
//      _remote_port1 = remote_port1;
//      _remote_port2 = remote_port2;
//      _state = VKVR_STATE_STOPPED;
//   };
//   return VKVR_OK;
//};
////-----------------------------------------------------------------------------
//// Конфигурирование ("пустой" метод).
//// Возвращает VKVR_OK в случае успеха, иначе код ошибки.
//int VKVRCapture::config()
//{
//   return VKVR_OK;
//};
////-----------------------------------------------------------------------------
//// Запуск захвата кадров во внутреннем потоке. Если функция обратного
//// вызова не задана, то захват не запустится, будет выполнен только
//// поиск камеры (можно использовать для проверки связи с камерой).
////    callback функция обратного вызова для передачи кадра,
////    param    указатель для передачи данных в функцию обратного вызова,
////    channel  номер канала, с которого будет передаваться видео (1 или 2),
////    ipaddr   IP адрес ВК в строковом виде ("X.X.X.X"),
////    auth     закодированная в Base64 строка авторизации, в том
////             виде, в котором она должна быть в пртоколе HTTP.
//// Возвращает VKVR_OK в случае успеха, иначе код ошибки.
//int VKVRCapture::play(CapCallback* callback, void* param,
//   int channel, const char* ipaddr, const char* auth)
//{
//    _max_msg_size = 0;
//   if (_state == VKVR_STATE_CAPTURING) stop();
//   AutoLock lock(&_cs);
//   if (_state == VKVR_STATE_UNINITIALIZED) return VKVR_ERR_UNINITIALIZED;
//   if (_state != VKVR_STATE_STOPPED) return VKVR_ERR_WRONG_STATE;
//  // if (callback == NULL) return VKVR_OK;
//   if ((ipaddr == NULL) || (channel < 1) || (channel > 2))
//      return VKVR_ERR_BAD_PARAMS;
//   strncpy(_host, ipaddr, 15);
//   _host[15] = 0;
//   _ipaddr = inet_addr(ipaddr);
//   if (auth == NULL)
//        _auth[0] = 0;
//   else
//   {
//      strncpy(_auth, auth, 33);
//      _auth[33] = 0;
//   };
//   _channel = channel;
//   _callback = callback, _param = param;
//   _terminate = false;
//
//
//   ////ВЫполняем создание подключения////
//
//     // Создание UDP сокета для приёма видеоданных с камеры.
//   _udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//   if ((int)_udp_socket < 0)
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   struct sockaddr_in sa;
//   sa.sin_port = 0;
//   sa.sin_family = AF_INET; // AF_INET - пространство имен Internet
//   sa.sin_addr.s_addr = INADDR_ANY;
//
//   // Привязка сокета с выделением ему не занятого локального порта.
//   // Добавлена для взаимодействия ПО с Брандмауэром Windows.
//   if (SOCKET_ERROR == bind(_udp_socket, (struct sockaddr*)&sa, sizeof(sa)))
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   sa.sin_port = htons((_channel == 1) ? _remote_port1 : _remote_port2);
//   sa.sin_family = AF_INET; // AF_INET - пространство имен Internet
//   sa.sin_addr.s_addr = _ipaddr;
//
//   // UDP сокет настраивается на приём данных от видеокамеры с заданного
//   // в play() IP адреса и заданного в init() UDP порта для заданного
//   // канала.
//   if (SOCKET_ERROR == connect(_udp_socket, (struct sockaddr*)&sa, sizeof(sa)))
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   // Определяется локальный UDP порт, выделенный ОС для созданного сокета.
//   int salen = sizeof(sa);
//   if (SOCKET_ERROR == getsockname(_udp_socket, (struct sockaddr*)&sa, &salen))
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//   _local_port = ntohs(sa.sin_port);
//
//   // Определяется максимальный размер данных, который можно пытаться
//   // прочитать с UDP сокета за один вызов recv().
//  // unsigned int max_msg_size = 0;
//   int len = sizeof(_max_msg_size);
//   if ((SOCKET_ERROR == getsockopt(_udp_socket, SOL_SOCKET, SO_MAX_MSG_SIZE,
//                                   (char*)&_max_msg_size, &len)) ||
//       (_max_msg_size < 1))
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   if (_max_msg_size > 65536) _max_msg_size = 65536;
//   unsigned int socket_buff_size = _max_msg_size*2;
//   // Устанавливается относительно небольшой буфер для приёма данных
//   // UDP сокетом. Небольшой размер обеспечит отсутсвие задержки при
//   // воспроизведении принятых видеоданных.
//   if (SOCKET_ERROR == setsockopt(_udp_socket, SOL_SOCKET, SO_RCVBUF,
//                   (const char*)&socket_buff_size, sizeof(socket_buff_size)))
//   {
//      error_callback(VKVR_ERR_NET_FAILED);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   // После создания UDP сокета в камеру посылается команда
//   // запуска передачи видео.
//   int req_result = http_request("play");
//   if (req_result != VKVR_OK)
//   {
//      error_callback(req_result);
//      return VKVR_ERR_BAD_THREAD;
//   };
//
//   // Отправка по созданному соединению данных для установления
//   // UDP соединения. Отправка пакета разрешит брандмауэрам
//   // пропускать UDP пакеты от камеры в ПЭВМ. Приём пакета
//   // камерой позволит переключить соединение на нужный порт в
//   // случае подключения через NAT.
//   char syn_buf[18] = "UDPVIDEOPLAY";
//   *(unsigned __int16*)&syn_buf[12] = htons(_local_port);
//   *(unsigned __int16*)&syn_buf[14] =
//      htons((_channel == 1) ? _remote_port1 : _remote_port2);
//   *(unsigned __int16*)&syn_buf[16] = 0;
//   send(_udp_socket, syn_buf, sizeof(syn_buf), 0);
//   send(_udp_socket, syn_buf, sizeof(syn_buf), 0);
//   send(_udp_socket, syn_buf, sizeof(syn_buf), 0);
//
//   int retval;
//   unsigned __int8* pffd9 = NULL;
//
//   // Обнуление количества накопленных в буфере байт.
//   _jpeg_size = 0;
//
//
//   /////////////////////////////////////////
//
//
//
//
//   // Проверка и освобождения дескриптора потока. Может потребоваться
//   // в случае останова потока вызовом stop() из самого потока - в
//   // этом случае поток закрывается сам, но дескриптор остаётся занятым.  
//   //if (_work_thread != NULL) CloseHandle(_work_thread);
//   //_work_thread = CreateThread(
//   //   NULL, 0, run_capture_work_routine, (LPVOID)this, 0, &_thread_id);
//   //if (_work_thread == NULL) return VKVR_ERR_BAD_THREAD;
//   //_state = VKVR_STATE_CAPTURING;
//   return VKVR_OK;
//};
////-----------------------------------------------------------------------------
//// Останов захвата с камеры. Может вызываться как в функции обратного
//// вызова так и из другого потока приложения.
//void VKVRCapture::stop()
//{
//   {
//      AutoLock lock(&_cs);
//      // Возможна ситуация вызова stop() в состоянии VKVR_STATE_STOPPED.
//      // Например после вызова stop() из обработчика поток завершается,
//      // в нём выполняется отправка команды по HTTP. Если в это время
//      // выполняется удаление объекта с вызовом free() и оттуда stop(),
//      // то надо дождаться завершения потока или уничтожить его.  
//      if ((_state != VKVR_STATE_CAPTURING) && (_state != VKVR_STATE_STOPPED))
//         return;
//      _terminate = true;
//      _callback = NULL;
//      _state = VKVR_STATE_STOPPED;
//   }
//   // Если функция вызвана в потоке захвата кадров (следовательно из
//   // функции обратного вызова), то поток закроется самостоятельно,
//   // см. DWORD WINAPI run_capture_work_routine(LPVOID lpParameter).
//   if (GetCurrentThreadId() != _thread_id)
//   {
//      // Ожидание завершения потока захвата кадров. Если в течении
//      // 3-х секунд поток не завершился - поток уничтожается.
//      if (_work_thread != NULL)
//      {
//         if (WaitForSingleObject(_work_thread, 3000) != WAIT_OBJECT_0)
//            TerminateThread(_work_thread, 0);
//         CloseHandle(_work_thread);
//         _work_thread = NULL;
//      };
//      AutoLock lock(&_cs);
//      if (_http_socket != INVALID_SOCKET)
//      {
//         closesocket(_http_socket);
//         _http_socket = INVALID_SOCKET;
//      };
//      if (_udp_socket != INVALID_SOCKET)
//      {
//         closesocket(_udp_socket);
//         _udp_socket = INVALID_SOCKET;
//      };
//   };
//};
////-----------------------------------------------------------------------------
//// Деинициализация класса (останов захвата, освобождение ресурсов).
//void VKVRCapture::free() throw()
//{ try {
//   if ((_state == VKVR_STATE_WSA_ERROR) ||
//       (_state == VKVR_STATE_UNINITIALIZED))
//      return;
//   stop();
//   AutoLock lock(&_cs);
//   if (_jpeg_data != NULL)
//   {
//      delete [] _jpeg_data;
//      _jpeg_data = NULL;
//   };
//   _state = VKVR_STATE_UNINITIALIZED;
//} catch (...) { } };
////-----------------------------------------------------------------------------
//// Возвращает текущее состояние класса.
//int VKVRCapture::state() const 
//{
//   return _state;
//};
////-----------------------------------------------------------------------------
//// Функция посылки HTTP запроса на запуск/останов передачи видео
//// по сети.
////    cmd строковая команда: "play" или "stop".
//// Возвращает VKVR_OK в случае успеха, иначе код ошибки.
//int VKVRCapture::http_request(char* cmd)
//{
//   // Создание TCP сокета для отправки CGI команды.
//   _http_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   if ((int)_http_socket < 0) return VKVR_ERR_NET_FAILED;
//
//   struct sockaddr_in sa;
//   sa.sin_addr.s_addr = _ipaddr;
//   sa.sin_family = AF_INET;
//   sa.sin_port = htons(80); // порт HTTP
//
//   // Подключение к камере к 80-му порту.
//   if (SOCKET_ERROR == connect(_http_socket, (struct sockaddr*)&sa, sizeof(sa)))
//   {
//      closesocket(_http_socket);
//      _http_socket = INVALID_SOCKET;
//      return VKVR_ERR_NO_CONNECT;
//   };
//
//   // Формирование пакета HTTP запроса с CGI командой.
//   char packet[1460];
//   if (_auth[0] == 0)
//      sprintf(packet,
//         "GET /udpvideo.cgi?ch=%d&sport=%d&cport=%d&cmd=%s HTTP/1.1\r\n"
//         "Connection: close\r\n"
//         "Host: %s\r\n"
//         "\r\n",
//         _channel, (_channel == 1) ? _remote_port1 : _remote_port2,
//         _local_port, cmd, _host);
//   else
//      sprintf(packet,
//         "GET /udpvideo.cgi?ch=%d&sport=%d&cport=%d&cmd=%s HTTP/1.1\r\n"
//         "Connection: close\r\n"
//         "Host: %s\r\n"
//         "Authorization: Basic %s\r\n"
//         "\r\n",
//         _channel, (_channel == 1) ? _remote_port1 : _remote_port2,
//         _local_port, cmd, _host, _auth);
//
//   // Отправка запроса.
//   if (SOCKET_ERROR == send(_http_socket, packet, (int)strlen(packet), 0))
//   {
//      closesocket(_http_socket);
//      _http_socket = INVALID_SOCKET;
//      return VKVR_ERR_SEND_FAILED;
//   };
//
//   // Получение ответа.
//   int retval = recv(_http_socket, packet, sizeof(packet), 0);
//
//   // Для простоты сокет просто закрывается без выполнения закрытия
//   // соединения вызовом "shutdown(_http_socket, SD_BOTH);".
//   closesocket(_http_socket);
//   _http_socket = INVALID_SOCKET;
//
//   if (SOCKET_ERROR == retval) return VKVR_ERR_RECV_FAILED;
//
//   // Проверяется получение ответа с кодом успешного выполнения запроса:
//   // "HTTP/1.1 2......." (например: "HTTP/1.1 204 No Content\r\n").
//   if ((retval < 10) || (packet[9] != '2')) return VKVR_ERR_RECV_FAILED;
//
//   return VKVR_OK;
//};
////-----------------------------------------------------------------------------
//// Функция поиска последовательности из 2-х байт в массиве.
//// Возвращает адрес найденных байт, либо NULL, если не были найдены. 
//unsigned __int8* find_code(unsigned __int8* from,
//   const unsigned __int8* to, unsigned __int8 code1, unsigned __int8 code2)
//{
//   while ((from + 1) < to)
//   {
//      if ((*from == code1) && (*(from + 1) == code2))
//         return from;
//      else
//         ++from;
//   };
//   return NULL;
//};
////---------------------------------------------------------------------------
//// Возвращает true, если по полученному указателю находится заголовок кадра,
//// и false в противном случае.
//bool is_begin(unsigned __int8* d)
//{
//   return
//      (*(unsigned __int32*)d == MFOURCC('H', 'R', 'K', 'o')) &&
//      (*(unsigned __int32*)(d + 4) == MFOURCC('n', 't', 'u', 'r')) &&
//      (*(unsigned __int16*)(d + 12) == (unsigned __int16)0xD8FF);
//};
////---------------------------------------------------------------------------
//// Функция ищет заголовок кадра и, если находит, сдвигает его в начало
//// буфера приёма данных.
//void VKVRCapture::find_begin()
//{
//   // Размер заголовка 12 байт + маркер JPEG "FF D8". Если накоплено
//   // меньше байт, то искать нечего.
//   if ((_jpeg_size >= 14) && !is_begin(_jpeg_data))
//   {
//      // Ищется заголовок кадра JPEG.
//      unsigned __int8* pffd8 = _jpeg_data + 12;
//      while (pffd8 != NULL)
//      {
//         ++pffd8;
//         pffd8 = find_code(pffd8, _jpeg_data + _jpeg_size, 0xFF, 0xD8);
//         if (pffd8 != NULL) if (is_begin(pffd8 - 12)) break;
//      };
//      if (pffd8 != NULL)
//      {
//         // Если заголовок найден, то отбрасывается информация до заголовка,
//         // данные начиная с заголовка сдвигаются в начало буфера.
//         pffd8 -= 12;
//         _jpeg_size -= (unsigned int)(pffd8 - _jpeg_data);
//         memmove(_jpeg_data, pffd8, _jpeg_size);
//      }
//      else
//      {
//         // Если заголовок не найден, то накопленная информация
//         // отбрасывается, за исключением последних 13-ти байт,
//         // в которых может находится начало заголовка. Последние
//         // 13 байт сдвигаются в начало буфера.
//         memmove(_jpeg_data, _jpeg_data + _jpeg_size - 13, 13);
//         _jpeg_size = 13;
//      };
//   };
//};
////-----------------------------------------------------------------------------
//// Вызов функции обратного вызова с кодом ошибки.
//void VKVRCapture::error_callback(int e)
//{
//   if (_callback != NULL)
//      _callback(_param, NULL, 0, e);
//   _callback = NULL;
//};
////-----------------------------------------------------------------------------
//// Функция потока захвата кадров.
//bool VKVRCapture::capture_work_routine()
//{
//    //_max_msg_size = 0;
//    unsigned __int8* pffd9 = NULL;
//    bool routine_ret = false;
// 
//  //while (!_terminate)
//   {
//       
//      // Считывается очередная часть видеоданных в незанятую часть буфера.
//      int retval = recv(_udp_socket,
//         (char*)(_jpeg_data + _jpeg_size), _max_msg_size, 0);
//      if (_terminate || (retval <= 0) || (retval == SOCKET_ERROR)) return false;
//
//      // Количество накопленных байт увеличивается на количество принятых байт.
//      _jpeg_size += retval;
//
//      // Заголовок кадра, если найден, сдвигается в начало буфера.
//      find_begin();
//
//      // Если найден заголовок кадра, то ищется его конец. Конец ищется
//      // после первых 1024 байт. В начальные байты пропускаются чтобы не
//      // перепутать конец кадра с какими-либо данными в заголовке EXIF.
//      // Максимальный размер заголовка на данный момент: 777 байт.
//      // Минимальный размер кадра на данный момент: 1875 байт. 
//      if ((_jpeg_size > 1024) &&
//          (*(unsigned __int16*)(_jpeg_data + 12) == 0xD8FF))
//      {
//         // Для ускорения процесса, конец кадра ищется либо с начала
//         // (если он ещё не искался), либо только во вновь полученных данных,
//         // либо вообще не ищется, если был найден раньше (маркер "JPEG FF D9"
//         // был найден, но эпилог кадра не был принят).
//         if ((pffd9 == _jpeg_data) ||
//             (_jpeg_size < (unsigned int)(1024 + 1 + retval)))
//            pffd9 = find_code(_jpeg_data + 1024,
//                              _jpeg_data + _jpeg_size, 0xFF, 0xD9);
//         else if (pffd9 == NULL)
//            pffd9 = find_code(_jpeg_data + _jpeg_size - 1024 - 1 - retval,
//                              _jpeg_data + _jpeg_size, 0xFF, 0xD9);
//         // Если найден конец кадра, то проверяется корректность кадра.
//         if ((pffd9 != NULL) && ((pffd9 + 2) <= (_jpeg_data + _jpeg_size)))
//         {
//            unsigned __int32 recv_jpeg_size =
//               (unsigned __int32)(pffd9 - (_jpeg_data + 12) + 2);
//            unsigned int remnant = (4 - (recv_jpeg_size % 4)) % 4;
//            // Кадр должен содержать 12 байт заголовка, собственно сам
//            // кадр от маркера FFD8 до маркера FFD9, поле выравнивания
//            // кадра до кратности 4-м байтам и 16 байт эпилога.
//            if ((pffd9 + 2 + remnant + 16) <= (_jpeg_data + _jpeg_size))
//            {
//               unsigned __int32* hr = (unsigned __int32*)_jpeg_data;
//               unsigned __int32* fr = (unsigned __int32*)(pffd9 + 2 + remnant);
//               unsigned __int32 real_jpeg_size = htonl(fr[3]);
//               // Корректность заголовка проверяется в "find_begin();",
//               // проверяется идентификатор эпилога, совпадение номеров кадров
//               // в заголовке и эпилоге, равенство размера принятого кадра
//               // размеру, указанному в эпилоге.
//               if ((recv_jpeg_size == real_jpeg_size) &&
//                   (fr[0] == MFOURCC('K', 'o', 'n', 't')) &&
//                   (fr[1] == MFOURCC('u', 'r', 'F', 'R')) &&
//                   (fr[2] == hr[2]) &&
//                   (_callback != NULL))
//                  // Корректно принятый кадр отправляется на обработку.
//               // return _jpeg_data + 12;
//
//                  _callback(_param, _jpeg_data + 12, recv_jpeg_size, VKVR_OK);
//               routine_ret = true;
//               // Не зависимо от корректности отбрасываем кадр и сдвигаем
//               // оставшиеся данные в начало буфера. Здесь можно было бы
//               // дополнительно проверить что если кадр некорректный,
//               // возможно потерян его конец и можно не отбрасывать данные,
//               // а поискать в них начало следующего кадра.
//               unsigned __int8* next_data = pffd9 + 2 + remnant + 16;
//               _jpeg_size -= (unsigned int)(next_data - _jpeg_data);
//               memmove(_jpeg_data, next_data, _jpeg_size);
//               pffd9 = _jpeg_data;
//            }; //
//         }; //
//
//         // Если конец кадра ещё не найден, а место в буфере уже заканчивается,
//         // то предполагаем либо слишком большой кадр, либо (что более
//         // вероятно) потерю последнего пакета кадра. Такой кадр отбрасывается.
//         while ((RECV_BUFF_SIZE - _jpeg_size) < _max_msg_size)
//         {
//            _jpeg_data[0] = 0;
//            // Заголовок следующего кадра, если найден,
//            // сдвигается в начало буфера.
//            find_begin();
//            pffd9 = _jpeg_data;
//         };
//      }; // if (найдено начало JPEG)
//   }; // while (!_terminate)
//
//   // В камеру посылается команда останова передачи видео.
//  // http_request("stop");
//
//  // closesocket(_udp_socket);
//  // _udp_socket = INVALID_SOCKET;
//
//   return routine_ret;
//};
////-----------------------------------------------------------------------------
//// Функция запуска потока захвата кадров.
///*
//DWORD WINAPI run_capture_work_routine(LPVOID lpParameter)
//{
//   VKVRCapture* vkvrcap = (VKVRCapture*)lpParameter;
//   vkvrcap->capture_work_routine();
//   AutoLock lock(&vkvrcap->_cs);
//   // Закрытие UDP сокета.
//   if (vkvrcap->_udp_socket != INVALID_SOCKET)
//   {
//      closesocket(vkvrcap->_udp_socket);
//      vkvrcap->_udp_socket = INVALID_SOCKET;
//   };
//   // Если завершение приёма выполняется не по вызову stop(), то нужно
//   // дополнительно изменить состояние объекта, оповестить через
//   // callback об ошибке и удалить Handle потока.
//   if (!vkvrcap->_terminate)
//   {
//      vkvrcap->_state = VKVR_STATE_STOPPED;
//      vkvrcap->error_callback(VKVR_ERR_CAP_FAILED);
//      CloseHandle(vkvrcap->_work_thread);
//      vkvrcap->_work_thread = NULL;
//   };
//   return 0;
//};*/
////-----------------------------------------------------------------------------
//
//
//
//
//
