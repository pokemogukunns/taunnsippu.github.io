QT += core gui
QT += network
TEMPLATE = app
SOURCES += \
    main.cpp \
    mainWindow.cpp \
    game.cpp \
    storage.cpp \
    castle.cpp \
    hut.cpp \
    market.cpp \
    resource.cpp \
    objElem.cpp \
    field.cpp \
    objectPic.cpp \
    rect.cpp

HEADERS += \
    town.h
RESOURCES += \
    images.qrc \
    images.qrc

FORMS += \
    dialog.ui \
    reqRes.ui \
    castleFunc.ui \
    storageFunc.ui \
    marketFunc.ui \
    settlementFunc.ui \
    hutFunc.ui \
    inform.ui \
    gameSet.ui \
    help.ui \
    production1.ui \
    production2.ui \
    noticeExit.ui \
    newProfile.ui \
    profiles.ui \
    myProfile.ui

RC_FILE = icon.rc
