set(CUR_DIR ${PROJECT_SOURCE_DIR}/stats)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS ALL)

target_link_libraries(app ${Boost_LIBRARIES})

target_sources(app PRIVATE
    ${CUR_DIR}/stats.hpp
)