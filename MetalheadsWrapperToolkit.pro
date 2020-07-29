TEMPLATE = lib
CONFIG += c++1z
CONFIG -= app_bundle
CONFIG -= qt

# For dynamic linking
LIBS += -ldl
# FreeImage
LIBS += -lfreeimage
# LibSndfile
LIBS += -lsndfile
# LibSamplerate
LIBS += -lsamplerate
# OpenMPT
LIBS += -lopenmpt
# FreeType
LIBS += -lfreetype
INCLUDEPATH += "/usr/include/freetype2"
# GIF
LIBS += -lgif
# WebP
LIBS += -lwebp
# Mono
LIBS += -L/usr/lib64/pkgconfig/../../lib64 -lmono-2.0 -lm -lrt -ldl -lpthread
INCLUDEPATH += "/usr/lib64/pkgconfig/../../include/mono-2.0"

SOURCES += \
    Io/StdStream.cpp \
    Wrappers/Audio/ModuleRenderer.cpp \
    Wrappers/Audio/Resampler.cpp \
    Wrappers/Audio/SoundfileWrapper.cpp \
    Wrappers/Graphics/FontWrapper.cpp \
    Wrappers/Graphics/GifWrapper.cpp \
    Wrappers/Graphics/ImageWrapper.cpp \
    Wrappers/Graphics/S3Wrapper.cpp \
    Wrappers/Graphics/WebpWrapper.cpp \
    Wrappers/Script/DynamicLibrary.cpp \
    Wrappers/Script/Mono/MonoAssembly.cpp \
    Wrappers/Script/Mono/MonoContext.cpp \
    Wrappers/Script/Mono/MonoImage.cpp

HEADERS += \
    Abstract/Global.hpp \
    Abstract/Graphics/ArrayTexture1D.hpp \
    Abstract/Graphics/ArrayTexture2D.hpp \
    Abstract/Graphics/PrimitiveType.hpp \
    Abstract/Graphics/Streamable.hpp \
    Abstract/Graphics/StreamingTexture1D.hpp \
    Abstract/Graphics/StreamingTexture2D.hpp \
    Abstract/Graphics/StreamingTexture3D.hpp \
    Abstract/Graphics/Texture.hpp \
    Abstract/Graphics/Texture1D.hpp \
    Abstract/Graphics/Texture2D.hpp \
    Abstract/Graphics/Texture3D.hpp \
    Abstract/Graphics/UniversalHandle.hpp \
    Abstract/Io/FIO.hpp \
    Abstract/Io/FIOSys.hpp \
    Io/StdStream.hpp \
    Wrappers/Audio/AudioPacket.hpp \
    Wrappers/Audio/ModuleRenderer.hpp \
    Wrappers/Audio/Resampler.hpp \
    Wrappers/Audio/SoundfileWrapper.hpp \
    Io/span.hpp \
    Wrappers/Graphics/FontWrapper.hpp \
    Wrappers/Graphics/GifWrapper.hpp \
    Wrappers/Graphics/ImageWrapper.hpp \
    Wrappers/Graphics/S3Wrapper.hpp \
    Wrappers/Graphics/Vector.hpp \
    Wrappers/Graphics/WebpWrapper.hpp \
    Wrappers/Script/DynamicLibrary.hpp \
    Wrappers/Script/Mono/MonoAssembly.hpp \
    Wrappers/Script/Mono/MonoContext.hpp \
    Wrappers/Script/Mono/MonoImage.hpp

DISTFILES += \
	README.md
