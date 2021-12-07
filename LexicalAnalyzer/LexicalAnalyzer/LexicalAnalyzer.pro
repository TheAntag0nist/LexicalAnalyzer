TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        Parser.cpp \
        Token.cpp \
        StateMachine.cpp

HEADERS += \
    Parser.h \
    StateMachine.h \
    Token.h \
    AnalyzeExceprions.h


