
add_executable(${PROJECT_NAME}_app main.cpp)


target_include_directories(${PROJECT_NAME}_app PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}_app PRIVATE ${PROJECT_SOURCE_DIR}/gui)

target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PROJECT_NAME}_gui)
target_link_libraries(${PROJECT_NAME}_app PRIVATE Qt6::Widgets)

target_compile_definitions(${PROJECT_NAME}_app PRIVATE
        ROENTGEN_VERSION="${PROJECT_VERSION}"
)
