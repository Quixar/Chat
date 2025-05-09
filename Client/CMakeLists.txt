cmake_minimum_required(VERSION 3.16)
project(Client VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем исходные файлы
set(SOURCES
    main.cpp
    StateMachine/RegistrationState.cpp
    StateMachine/LoginState.cpp
    StateMachine/StateMachine.cpp
    NetClient/NetClient.cpp
)

set(HEADERS
    StateMachine/RegistrationState.h
    StateMachine/LoginState.h
    StateMachine/StateMachine.h
)

# Создаем исполняемый файл
add_executable(Client ${SOURCES} ${HEADERS})

# Находим SFML 3 и подключаем необходимые компоненты
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Network)

# Подключаем SFML к проекту
target_link_libraries(Client PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Network)

# Указываем директории с заголовочными файлами
target_include_directories(Client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/StateMachine
)

