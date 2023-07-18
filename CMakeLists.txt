cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HelloThere C CXX)

if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Threads REQUIRED)

option(protobuf_MODULE_COMPATIBLE TRUE)

# find_package(Protobuf CONFIG REQUIRED)
# message(STATUS "Using protobuf ${Protobuf_VERSION}")

# find_package(gRPC CONFIG REQUIRED)
# message(STATUS "Using gRPC ${gRPC_VERSION}")

if(WIN32)

    set( RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/ )
    set( THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/ )

    set( GRPC_ZIP_DIR ${RESOURCES_DIR}/grpc/win/ )
    set( GRPC_ZIP_OUTPUT_DIR ${THIRD_PARTY_DIR}/grpc/win/ )

    configure_file(${RESOURCES_DIR}/Script.txt ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/Script.txt COPYONLY)
    file(MAKE_DIRECTORY ${GRPC_ZIP_OUTPUT_DIR})

    add_custom_target( unZip ALL )
    add_custom_command( 
        TARGET unZip PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${GRPC_ZIP_DIR}/v1.56.0.zip
        WORKING_DIRECTORY ${GRPC_ZIP_OUTPUT_DIR}
        DEPENDS ${GRPC_ZIP_DIR}/v1.56.0.zip
        COMMENT "Unpacking grpc..."
        VERBATIM
    )

    set( GRPC_PATH ${GRPC_ZIP_OUTPUT_DIR}/v1.56.0/ )

    set( PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos/ )
    set( PROTO_DST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gen/proto/ )
    set( PROTO_BIN ${GRPC_PATH}/bin/protoc.exe )
    set( GRPC_PLUGIN ${GRPC_PATH}/bin/grpc_cpp_plugin.exe )

    set( GRPC_PROTO_OUTPUT 
         ${PROTO_DST_DIR}/HelloThere.pb.h
         ${PROTO_DST_DIR}/HelloThere.pb.cc
         ${PROTO_DST_DIR}/HelloThere.grpc.pb.h
         ${PROTO_DST_DIR}/HelloThere.grpc.pb.cc
    )

    add_custom_command(
        OUTPUT ${GRPC_PROTO_OUTPUT}
        COMMAND ${PROTO_BIN}
        ARGS -I "${PROTO_SRC_DIR}" 
             --grpc_out "${PROTO_DST_DIR}"
             --cpp_out "${PROTO_DST_DIR}" 
             --plugin=protoc-gen-grpc="${GRPC_PLUGIN}"
             "${PROTO_SRC_DIR}/HelloThere.proto"
        DEPENDS "${PROTO_SRC_DIR}/HelloThere.proto"
        COMMENT "Generating protos..."
    )

    include_directories( "${GRPC_PATH}/include" "${PROTO_DST_DIR}" )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

    set( PROTO_DEPS
        "${GRPC_PATH}/lib/grpc++_reflection.lib"
        "${GRPC_PATH}/lib/grpc++.lib"
        "${GRPC_PATH}/lib/libprotobuf.lib"
    )
    add_library( proto ${GRPC_PROTO_OUTPUT} )
    target_link_libraries( proto ${PROTO_DEPS} )

    add_executable( Demo 
                    src/demo/main.cpp 
                    src/demo/process/win/Process.cpp
                    src/utils/win/system.cpp
    )
    target_include_directories( Demo PUBLIC src/ )

    set( GRPC_DEPS
         "${GRPC_PATH}/lib/absl_*.lib"
         "${GRPC_PATH}/lib/grpc.lib" 
         "${GRPC_PATH}/lib/cares.lib"
         "${GRPC_PATH}/lib/address_sorting.lib"
         "${GRPC_PATH}/lib/re2.lib"
         "${GRPC_PATH}/lib/upb.lib"
         "${GRPC_PATH}/lib/gpr.lib"
         "${GRPC_PATH}/lib/ssl.lib"
         "${GRPC_PATH}/lib/crypto.lib"
         "${GRPC_PATH}/lib/utf8_validity.lib"
         "${GRPC_PATH}/lib/zlibstatic.lib"
    )

    add_executable( Client src/client/main.cpp )
    target_link_libraries( Client proto ${GRPC_DEPS} ${PROTO_DEPS} )
    target_include_directories( Client PUBLIC src/ )

    add_executable( Server src/server/main.cpp src/utils/win/system.cpp src/server/script_reader.cpp src/server/ServiceImpl.cpp )
    target_link_libraries( Server proto ${GRPC_DEPS} ${PROTO_DEPS} )
    target_include_directories( Server PUBLIC src/ )
else()
    message( FATAL_ERROR "Only windows platform is supported" )
endif(WIN32)