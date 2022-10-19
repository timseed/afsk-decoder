QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        AfskDecoder.cpp \
        BitDetector.cpp \
        ByteDetector.cpp \
        MessageDecoder.cpp \
        ZeroCrossingDetector.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AfskDecoder.h \
    AudioFile.h \
    BitDetector.h \
    ByteDetector.h \
    MessageDecoder.h \
    ZeroCrossingDetector.h \
    data_types.h
