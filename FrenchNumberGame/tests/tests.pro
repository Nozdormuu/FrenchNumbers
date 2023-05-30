QT += testlib
QT += gui network
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app
SOURCES += tst_testnumberconversion.cpp ../MainApp/numberconverter.cpp
HEADERS += ../MainApp/numberconverter.h
INCLUDEPATH += ../MainApp/

RESOURCES += \
    resources.qrc
