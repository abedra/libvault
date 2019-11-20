

add_executable(example example.cpp)
target_include_directories(example PRIVATE ../include)
target_link_libraries(example vault)



