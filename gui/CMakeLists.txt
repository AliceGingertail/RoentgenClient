
add_library(${PROJECT_NAME}_gui OBJECT MainWindow.cpp
        widgets/help_popup/HelpPopup.cpp
        widgets/help_popup/HelpPopup.h)


target_include_directories(${PROJECT_NAME}_gui PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_gui PRIVATE Qt6::Widgets)
