project(runnermodel)

set(runnermodel_SRCS
    runnermodel.cpp
    runnermodelplugin.cpp
    )
ecm_qt_declare_logging_category(runnermodel_SRCS HEADER krunner_debug.h IDENTIFIER KRUNNER CATEGORY_NAME org.kde.krunner)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/runnermodel)

