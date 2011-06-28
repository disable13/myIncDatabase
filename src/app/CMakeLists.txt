cmake_minimum_required(VERSION 2.6)
IF (COMMAND cmake_policy)
        cmake_policy (SET CMP0003 NEW)
ENDIF (COMMAND cmake_policy)
#
PROJECT (myinc)
# Compiler
IF (MSVC)
        SET (CMAKE_CXX_FLAGS "/GX /W3")
        SET (CMAKE_CXX_FLAGS_RELEASE "/Ob2 /O2 /Ot /Oi /Oy /GT /GL /MD")
        SET (CMAKE_EXE_LINKER_FLAGS "/entry:mainCRTStartup")
        ADD_DEFINITIONS (-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
ENDIF (MSVC)
IF (UNIX)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fno-strict-aliasing")
        SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
        IF (EXTRA_WARNINGS)
                SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
        ENDIF (EXTRA_WARNINGS)
        IF (USE_PIPE)
                SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
        ENDIF (USE_PIPE)
ENDIF (UNIX)
IF (WIN32 AND NOT MSVC)
        SET (CMAKE_CXX_FLAGS "-Wall")
ENDIF (WIN32 AND NOT MSVC)
#
SET (LC_PLUGINS_DEST "${LIBDIR}/myinc")
#
FIND_PACKAGE (Qt4 REQUIRED)
#
SET (QT_USE_QTCORE TRUE)
SET (QT_USE_QTUI TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTUITOOLS TRUE)
INCLUDE (${QT_USE_FILE})
#
SET (SRCS
     main.cpp
     QProgressIndicator.cpp
     core/dproject.cpp
     core/dnamespace.cpp
     core/myincapplication.cpp
     core/dsystemfuncs.cpp
     core/dauth.cpp
     core/durihelper.cpp
     core/ddebug.cpp
     ui/mainwindow.cpp
     ui/dfooter.cpp
     ui/ddbconfig.cpp
     ui/dhomescreen.cpp
     ui/dworkwidget.cpp
     ui/dsqlquertyviewer.cpp
     ui/qaboutwidget.cpp
     ui/ddebugwidget.cpp
)

SET (HEADERS
     errors.h
     QProgressIndicator.h
     core/dproject.h
     core/dnamespace.h
     core/myincapplication.h
     core/dsystemfuncs.h
     core/dauth.h
     core/durihelper.h
     core/ddebug.h
     ui/mainwindow.h
     ui/dfooter.h
     ui/ddbconfig.h
     ui/dhomescreen.h
     ui/dworkwidget.h
     ui/dsqlquertyviewer.h
     ui/qaboutwidget.h
     ui/ddebugwidget.h
)

SET (RESOURCES
        ../share/resource.qrc
)
#
QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_ADD_RESOURCES (RCC ${RESOURCES})
#
IF (UNIX)
        ADD_EXECUTABLE (myinc
                ${SRCS}
                ${MOC_SRCS}
                ${RCC}
                )
ELSE (UNIX)
        ADD_EXECUTABLE (myinc WIN32
                ${SRCS}
                ${MOC_SRCS}
                ${RCC}
                )
ENDIF (UNIX)
#
TARGET_LINK_LIBRARIES(myinc
                        ${QT_LIBRARIES}
                        ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
#
INSTALL (TARGETS myinc DESTINATION bin)
INSTALL (DIRECTORY plugininterface/ DESTINATION include/myinc/
        PATTERN ".h")
IF (UNIX)
        INSTALL (FILES freedesktop/myinc.desktop DESTINATION share/applications)
ENDIF (UNIX)

