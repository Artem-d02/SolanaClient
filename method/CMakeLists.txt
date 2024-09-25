set(CUR_DIR ${PROJECT_SOURCE_DIR}/method)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS ALL)

target_link_libraries(app ${Boost_LIBRARIES})

target_sources(app PRIVATE
    ${CUR_DIR}/get_base.cpp
    ${CUR_DIR}/get_base.hpp
    ${CUR_DIR}/get_methods.cpp
    ${CUR_DIR}/get_methods.hpp
    ${CUR_DIR}/constants.hpp
    ${CUR_DIR}/constants.cpp
)