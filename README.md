# Build

- Установите Conan и добавьте дефолтный профиль
```sh
pip intall conan
conan profile detect
```

- Запустите сборку

```sh
conan build . -b=missing
```

- Запустите файл

```sh
./build/Debug/BlockToC ./data/sample.xml -o ./data/sample.cpp
```

- Запуск тестов
Для запуска тестов написан скрипт на `Powershell` и `Bash`:

```sh
./run_test.sh
```

Также там можно подсмотреть как настраивать параметры сборки (профиль, включение/выключение тестов).
