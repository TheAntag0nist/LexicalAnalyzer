TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Source.cpp \
        LexicAnalyzer.cpp \
        Token.cpp

HEADERS += \
    LexicAnalyzer.h \
    StateMachine.h \
    Token.h \
    AnalyzeExceprions.h


