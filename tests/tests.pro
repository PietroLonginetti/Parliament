QT += gui testlib charts

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


HEADERS +=  ../app/dialogvoteresults.h \
            ../app/mainwindow.h \
            ../app/parliament.h \
            ../app/party.h \
            ../app/slice.h \
            ../app/slicecontrolwidget.h \
            ../app/slicepartycontroller.h \
            ../app/votewidget.h

SOURCES +=  tst_testparliament.cpp \
            ../app/dialogvoteresults.cpp \
            ../app/mainwindow.cpp \
            ../app/parliament.cpp \
            ../app/party.cpp \
            ../app/slice.cpp \
            ../app/slicecontrolwidget.cpp \
            ../app/slicepartycontroller.cpp \
            ../app/votewidget.cpp \

FORMS +=    ../app/dialogvoteresults.ui \
            ../app/mainwindow.ui \
            ../app/slicecontrolwidget.ui \
            ../app/votewidget.ui
