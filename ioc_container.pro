TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += ioc_container.hpp

SOURCES += main.cpp \
    demos/demo1.cpp \
    demos/demo2.cpp
