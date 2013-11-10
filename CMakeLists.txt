cmake_minimum_required(VERSION 2.8)
set(Boost_DEBUG 1) 
project(pbook)

find_package(Qt4 REQUIRED)
include(FindProtobuf)
# find_package(ProtocolBuffers)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.46 COMPONENTS system)
 
QT4_WRAP_CPP(pbook_headers_moc mainwindow.h)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS}) 


QT4_WRAP_UI(pbook_forms_headers mainwindow.ui)
# QT4_ADD_RESOURCES(helloworld_RESOURCES_RCC ${helloworld_RESOURCES})

ADD_EXECUTABLE(pbook main.cpp mainwindow.cpp 
    ${pbook_headers_moc}
    ${pbook_forms_headers})

include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS}) 
TARGET_LINK_LIBRARIES(pbook 
	${QT_LIBRARIES}
	${PROTOBUF_LIBRARIES})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS messages.proto)

add_executable(bar bar.cpp mocknetwork.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(bar ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES})

