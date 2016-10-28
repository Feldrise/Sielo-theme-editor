QT+=widgets
QT += webenginewidgets

SOURCES += \
    src/Widgets/ToolBar.cpp \
    src/Main.cpp \
    src/MainWindow.cpp \
    src/ThemeManager.cpp

HEADERS += \
    includes/Widgets/ToolBar.hpp \
    includes/MainWindow.hpp \
    includes/ThemeManager.hpp


release: DESTDIR = bin
debug:   DESTDIR = bin

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

RESOURCES += \
    default.qrc
