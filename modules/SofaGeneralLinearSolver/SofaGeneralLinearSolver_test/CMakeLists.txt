cmake_minimum_required(VERSION 2.8.12)

project(SofaBaseLinearSolver_test)

set(SOURCE_FILES
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaTest SofaBaseLinearSolver)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


