
add_library(${PROJECT_NAME}_gui OBJECT
        widgets/main_window/MainWindow.cpp
        widgets/main_window/MainWindow.h
        widgets/image_view/ImageView.cpp
        widgets/help_popup/HelpPopup.cpp
        widgets/help_popup/HelpPopup.h)


target_link_libraries(${PROJECT_NAME}_gui PRIVATE Qt6::Widgets ${PROJECT_NAME}_lib)
