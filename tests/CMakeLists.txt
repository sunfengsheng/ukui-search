add_executable(runnermodeltest
    runnermodeltest.cpp
    ../src/declarative/runnermodel.cpp
    mainrunner.cpp
    filtermodel.cpp
    pinyin.cpp
    settingmodel.cpp
    res.qrc)
target_link_libraries(runnermodeltest Qt5::Widgets KF5::Service KF5Runner Qt5::Xml)
#ecm_mark_as_test(runnermodeltest)

