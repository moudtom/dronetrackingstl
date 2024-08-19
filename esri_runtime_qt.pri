#-------------------------------------------------
#  Copyright 2019 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

ESRIRUNTIME_QT_VERSION = 100.11.2
ESRIRUNTIME_QT_MAJOR_VERSION = 100
ESRIRUNTIME_QT_MINOR_VERSION = 11
ESRIRUNTIME_QT_PATCH_VERSION = 2

QT += gui network positioning sensors multimedia

unix:!macx {
    message("ArcGIS Runtime for Unix ("$$QT_ARCH")")
    PLATFORM = "linux"
}

macx {
    message("ArcGIS Runtime for Mac OS X ("$$QT_ARCH")")
    PLATFORM = "macOS"
}

win32 {
    message("ArcGIS Runtime for Windows ("$$QT_ARCH")")
    PLATFORM = "windows"
}

message("Version " $$ESRIRUNTIME_QT_VERSION)

#-------------------------------------------------
# Compiler options
#-------------------------------------------------

SDK_INSTALL_DIR = "C:/Program Files (x86)/ArcGIS SDKs/Qt100.11.2"

unix:!macx{
    eval(QMAKE_TARGET.arch = ""):{
        # QMAKE_TARGET.arch isn't set properly on Linux.
        # If we get a bitset-specific mkspec, use it
        linux-g++-32:QMAKE_TARGET.arch = x86
        linux-g++-64:QMAKE_TARGET.arch = x86_64

        # If we get a generic one, then determine the
        # arch of the machine and assign
        linux-g++:{
            ARCH = $$system(uname -m) # i686 or x86_64
            contains(ARCH, x86_64):{
                QMAKE_TARGET.arch = x86_64
            }
            else{
                QMAKE_TARGET.arch = x86
            }
        } # linux-g++
    } # eval
} # unix

macx {
    QMAKE_POST_LINK  += install_name_tool -change libEsriCommonQt.dylib \"$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x64/lib/libEsriCommonQt.dylib\" $${TARGET}.app/Contents/MacOS/$${TARGET} $$escape_expand(\n\t)
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}

#-------------------------------------------------
#  Headers
#-------------------------------------------------

INCLUDEPATH += "$${SDK_INSTALL_DIR}/sdk/include"

!macx {
  INCLUDEPATH += "$${SDK_INSTALL_DIR}/sdk/include/LocalServer"
}

#-------------------------------------------------
#  Libraries
#-------------------------------------------------

contains(QMAKE_TARGET.arch, x86):{
  unix:!macx{
    LIBS += \
      -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x86/lib \
      -lEsriRuntimeQt \
      -lEsriCommonQt \
      -lruntimecore
  }
}
else {
  unix:!macx{
    LIBS += \
      -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x64/lib \
      -lEsriRuntimeQt \
      -lEsriCommonQt \
      -lruntimecore
  }
}

macx:{
  LIBS += \
      -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x64/lib \
      -lEsriCommonQt \
      -lEsriRuntimeQt
}

win32:{
  CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += /FS
    QMAKE_LFLAGS += /IGNORE:4099
  }

  contains(QT_ARCH, x86_64):{
    LIBS +=  \
      -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x64/lib \
      -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x64/bin
  }
  else {
     LIBS +=  \
       -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x86/lib \
       -L$${SDK_INSTALL_DIR}/sdk/$${PLATFORM}/x86/bin
  }

  CONFIG(debug, debug|release) {
    LIBS += \
      -lEsriCommonQtd \
      -lEsriRuntimeQtd
  } else {
    LIBS += \
      -lEsriCommonQt \
      -lEsriRuntimeQt
  }
}
