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