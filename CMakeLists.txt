cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
set(Boost_DEBUG 1) 
project(pbook)

find_package(Qt4 REQUIRED)
include(FindProtobuf)
# find_package(ProtocolBuffers)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.46 COMPONENTS system unit_test_framework REQUIRED)
 
QT4_WRAP_CPP(pbook_headers_moc mainwindow.h)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS}) 
ADD_DEFINITIONS(-std=c++0x)

QT4_WRAP_UI(pbook_forms_headers mainwindow.ui)
# QT4_ADD_RESOURCES(helloworld_RESOURCES_RCC ${helloworld_RESOURCES})

if (WIN32)
	include_directories (lib/nacl/win32/include)
	link_directories(lib/nacl/win32/lib)
elseif ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	include_directories (lib/nacl/amd64/include)
	link_directories(lib/nacl/amd64/lib)
ELSE()
	include_directories (lib/nacl/x86/include)
	link_directories(lib/nacl/x86/lib)
ENDIF()

if(WIN32)
	set(RNGSRC cryptgenrandom.c)
ELSE()
	set(RNGSRC devurandom.cpp)
ENDIF()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

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

set(APP_SRCS client.cpp server.cpp crypto.cpp arpencrypt.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${RNGSRC})
set(TEST_SRCS mocknetwork.cpp)
add_executable(tests tests.cpp ${TEST_SRCS} ${APP_SRCS})

add_executable(bar bar.cpp ${TEST_SRCS} ${APP_SRCS})
target_link_libraries(tests ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES} pthread nacl)
target_link_libraries(bar ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES} pthread nacl)

