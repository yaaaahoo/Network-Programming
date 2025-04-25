add_executable(iov_client iov_client.cpp)
target_link_libraries(iov_client common)

add_executable(iov_server iov_server.cpp)
target_link_libraries(iov_server common)