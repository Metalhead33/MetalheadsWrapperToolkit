TEMPLATE = lib
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lfreeimage -lsndfile -lfreetype -lsamplerate
INCLUDEPATH += "/usr/include/freetype2"

SOURCES += \
    Io/StdStream.cpp \
    Wrappers/MhImageWrapper.cpp \
    Wrappers/MhSoundfileWrapper.cpp \
    Wrappers/MhResampler.cpp \
    Wrappers/MhFontWrapper.cpp

HEADERS += \
    Io/FIO.hpp \
    Io/FIOSys.hpp \
    Io/StdStream.hpp \
    Io/Global.hpp \
    Wrappers/MhImageWrapper.hpp \
    Wrappers/MhSoundfileWrapper.hpp \
    Wrappers/MhResampler.hpp \
    Wrappers/MhFontWrapper.hpp \
    Io/span.hpp \
    Wrappers/MhVector.hpp
