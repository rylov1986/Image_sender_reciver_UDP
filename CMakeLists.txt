cmake_minimum_required (VERSION 3.1)
project (bitmap_picture_sender_Qt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory (udp_client)
add_subdirectory (udp_server)
