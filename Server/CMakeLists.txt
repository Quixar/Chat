cmake_minimum_required(VERSION 3.16)
project(ChatServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к MySQL Connector и SFML
set(MYSQL_INCLUDE_DIR /opt/homebrew/Cellar/mysql-connector-c++/9.3.0/include)
set(MYSQL_LIB_DIR /opt/homebrew/Cellar/mysql-connector-c++/9.3.0/lib)

set(SFML_INCLUDE_DIR /opt/homebrew/Cellar/sfml/3.0.0_1/include)
set(SFML_LIB_DIR /opt/homebrew/Cellar/sfml/3.0.0_1/lib)

# Включаем директории
include_directories(${MYSQL_INCLUDE_DIR} ${SFML_INCLUDE_DIR} ./Entities)

# Подключаем исходники
add_executable(server
    main.cpp
    Entities/DataContext.cpp
    Entities/User.cpp
    Entities/UserRole.cpp
    NetServer/server.cpp
)

# Линкуем библиотеки
target_link_directories(server PRIVATE ${MYSQL_LIB_DIR} ${SFML_LIB_DIR})
target_link_libraries(server
    mysqlcppconnx
    sfml-graphics
    sfml-network
    sfml-window
    sfml-system
)

# Указываем путь к .dylib при запуске (rpath)
set_target_properties(server PROPERTIES
    INSTALL_RPATH "${MYSQL_LIB_DIR};${SFML_LIB_DIR}"
)

