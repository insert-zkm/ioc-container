TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += ioc_container.hpp \
    test/demo1.hpp \
    test/demo2.hpp \
    test/demo3.hpp \
    test/test.hpp

SOURCES += main.cpp
