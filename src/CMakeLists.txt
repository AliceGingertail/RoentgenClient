
add_library(${PROJECT_NAME}_lib STATIC FileParser.cpp)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_lib PRIVATE ${ITK_LIBRARIES})
