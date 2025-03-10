# Oat++ HTTP Server

## 📌 Установка Oat++
```sh
git clone https://github.com/oatpp/oatpp
cd oatpp
mkdir build && cd build
cmake ..
make install
```

## 🚀 Компиляция сервера
```sh
g++ -std=c++17 -o server oatpp_web_server.cpp -I/usr/local/include -L/usr/local/lib -loatpp
```

## 🏃 Запуск сервера
```sh
./server
```

## 📜 Проверка логов
```sh
curl http://localhost:1616/log
```

## 📤 Загрузка файла на сервер
```sh
curl -X POST -F "file=@myTestFile.txt" http://localhost:1616/upload
```

## 🔥 Доступ к Swagger UI
Swagger UI доступен по адресу:
```
http://localhost:1616/swagger/ui
```

