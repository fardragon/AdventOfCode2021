

add_executable(day24 main.cpp Instruction.cpp ALU.cpp)
target_link_libraries(day24 PRIVATE shared_lib tbb)