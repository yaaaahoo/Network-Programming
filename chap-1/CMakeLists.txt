add_executable(tcp_client tcp_client.cpp)
target_link_libraries(tcp_client common)

add_executable(tcp_server tcp_server.cpp)
target_link_libraries(tcp_server common)