cmake_minimum_required(VERSION 3.21)
project(Electronics-service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(Qt6Hello
  main2.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  ../cpp/cerereR.cpp
  ../cpp/lucruAngajati.cpp
  ../cpp/lucruElectrocasnice.cpp
  ../cpp/angajat.cpp
  ../cpp/prelucrareCereri.cpp
  ../cpp/raporturi.cpp
)

target_link_libraries(Qt6Hello PRIVATE Qt6::Widgets)
