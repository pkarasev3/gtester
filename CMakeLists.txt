project(gtester)

cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED QtCore QtXml QtGui QtNetwork)

include_directories(${QT_HEADERS_DIR})
foreach(X  QtCore QtXml QtGui QtNetwork )
    include_directories(${QT_HEADERS_DIR}/${X})
    message(STATUS  "including path: ${QT_HEADERS_DIR}/${X}")
endforeach()

link_directories(${QT_LIBRARY_DIR})

add_definitions(-DQT_DLL)

set(QT_WRAP_UI "YES")
file(GLOB  XUiHdrs  ui/*.ui)
file(GLOB  XMocHdrs
    src/mainwindow.h  
    src/executablebase.h  
    src/executabletester.h  
    src/testmodel.h  
    src/executabletestrunner.h  
    src/application.h  
    src/defaultmenu.h  
    src/globalprogress.h
    )
file(GLOB XSrc src/*.cpp src/*.h)

QT4_WRAP_UI(XUiSrcs ${XUiHdrs})
QT4_WRAP_CPP(XMocSrcs ${XMocHdrs})
message(STATUS "UI hdrs = ${XUiSrcs} \r\n"  "Mochdrs = ${XMocSrcs}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src  ${CMAKE_CURRENT_SOURCE_DIR}/ui)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src  ${CMAKE_CURRENT_BINARY_DIR}/ui)

SET(X_QRC_FILES    resources/icons.qrc  )

QT4_ADD_RESOURCES(XResources   ${X_QRC_FILES})
               
add_executable(gtester ${XUiSrcs} ${XMocSrcs}  ${XSrc})
set(QT_LIBRARIES  QtCore QtXml QtGui QtNetwork )
message(STATUS  "qt libs:  ${QT_LIBRARIES}")
target_link_libraries(gtester  ${QT_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR})
