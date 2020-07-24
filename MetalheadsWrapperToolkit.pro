TEMPLATE = lib
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lfreeimage -lsndfile -lfreetype -lsamplerate -lopenmpt
INCLUDEPATH += "/usr/include/freetype2"

SOURCES += \
    Io/StdStream.cpp \
    Wrappers/Audio/ModuleRenderer.cpp \
    Wrappers/Audio/Resampler.cpp \
    Wrappers/Audio/SoundfileWrapper.cpp \
    Wrappers/Graphics/FontWrapper.cpp \
    Wrappers/Graphics/ImageWrapper.cpp

HEADERS += \
    Io/FIO.hpp \
    Io/FIOSys.hpp \
    Io/StdStream.hpp \
    Io/Global.hpp \
    Wrappers/Audio/AudioPacket.hpp \
    Wrappers/Audio/ModuleRenderer.hpp \
    Wrappers/Audio/Resampler.hpp \
    Wrappers/Audio/SoundfileWrapper.hpp \
    Io/span.hpp \
    Wrappers/Graphics/FontWrapper.hpp \
    Wrappers/Graphics/ImageWrapper.hpp \
    Wrappers/Graphics/Vector.hpp
