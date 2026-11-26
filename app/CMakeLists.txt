
add_executable(${PROJECT_NAME}_app main.cpp)

qt_add_resources(${PROJECT_NAME}_app app_theme
        PREFIX "/theme"
        BASE "${PROJECT_SOURCE_DIR}/gui/themes"
        FILES "${PROJECT_SOURCE_DIR}/gui/themes/medical.qss")


target_include_directories(${PROJECT_NAME}_app PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME}_app PRIVATE ${PROJECT_SOURCE_DIR}/gui)

target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PROJECT_NAME}_gui)
target_link_libraries(${PROJECT_NAME}_app PRIVATE Qt6::Widgets)

target_compile_definitions(${PROJECT_NAME}_app PRIVATE
        ROENTGEN_VERSION="${PROJECT_VERSION}"
)
