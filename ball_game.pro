TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/SFML
LIBS += -lsfml-window -lsfml-system -lsfml-graphics -lsfml-audio -lsfml-network

SOURCES += \
        main.cpp
