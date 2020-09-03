#-------------------------------------------------
#
# Project created by QtCreator 2018-07-15T11:12:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Map3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
#        D:\\develope\\OPENCV310\\opencv\\build\\include\\opencv  \
#        D:\\develope\\OPENCV310\\opencv\\build\\include\\opencv2 \
#        D:\\develope\\OPENCV310\\opencv\\build\\include \
#        D:\\develope\\NetCDF\\netCDF4.6.1\\include \
LIBS += \
#        D:\\develope\\OPENCV310\\opencv\\build\\x64\\vc14\\lib\\opencv_world310.lib \
#        D:\\develope\\NetCDF\\netCDF\\lib\\netcdf.lib


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myitem.cpp \
    myview.cpp \
    hanshuku.cpp \
    dikongjiliu.cpp \
    handle.cpp \
    tjuwind.cpp \
    f_wind.cpp \
    qwinddialog.cpp \
    dkjlhanshu.cpp \
    cxx4/ncAtt.cpp \
    cxx4/ncByte.cpp \
    cxx4/ncChar.cpp \
    cxx4/ncCheck.cpp \
    cxx4/ncCompoundType.cpp \
    cxx4/ncDim.cpp \
    cxx4/ncDouble.cpp \
    cxx4/ncEnumType.cpp \
    cxx4/ncException.cpp \
    cxx4/ncFile.cpp \
    cxx4/ncFloat.cpp \
    cxx4/ncGroup.cpp \
    cxx4/ncGroupAtt.cpp \
    cxx4/ncInt.cpp \
    cxx4/ncInt64.cpp \
    cxx4/ncOpaqueType.cpp \
    cxx4/ncShort.cpp \
    cxx4/ncString.cpp \
    cxx4/ncType.cpp \
    cxx4/ncUbyte.cpp \
    cxx4/ncUint.cpp \
    cxx4/ncUint64.cpp \
    cxx4/ncUshort.cpp \
    cxx4/ncVar.cpp \
    cxx4/ncVarAtt.cpp \
    cxx4/ncVlenType.cpp \
    qshikuangdialog.cpp \
    Line_jicao.cpp \
    pressure.cpp \
    qdatadialog.cpp \
    qcyclonedialog.cpp

HEADERS += \
        mainwindow.h \
    myitem.h \
    myview.h \
    hanshuku.h \
    dikongjiliu.h \
    handle.h \
    tjuwind.h \
    f_wind.h \
    qwinddialog.h \
    dkjlhanshu.h \
    cxx4/ncAtt.h \
    cxx4/ncByte.h \
    cxx4/ncChar.h \
    cxx4/ncCheck.h \
    cxx4/ncCompoundType.h \
    cxx4/ncDim.h \
    cxx4/ncDouble.h \
    cxx4/ncEnumType.h \
    cxx4/ncException.h \
    cxx4/ncFile.h \
    cxx4/ncFloat.h \
    cxx4/ncGroup.h \
    cxx4/ncGroupAtt.h \
    cxx4/ncInt.h \
    cxx4/ncInt64.h \
    cxx4/ncOpaqueType.h \
    cxx4/ncShort.h \
    cxx4/ncString.h \
    cxx4/ncType.h \
    cxx4/ncUbyte.h \
    cxx4/ncUint.h \
    cxx4/ncUint64.h \
    cxx4/ncUshort.h \
    cxx4/ncVar.h \
    cxx4/ncVarAtt.h \
    cxx4/ncVlenType.h \
    cxx4/netcdfcpp.h \
    qshikuangdialog.h \
    Line_jicao.h \
    pressure.h \
    qdatadialog.h \
    qcyclonedialog.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    test.py





win32:CONFIG(release, debug|release): LIBS += -LD:/Develope/NETCDF/lib/ -lnetcdf
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Develope/NETCDF/lib/ -lnetcdf

INCLUDEPATH += D:/Develope/NETCDF/include
DEPENDPATH += D:/Develope/NETCDF/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/NETCDF/lib/libnetcdf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/NETCDF/lib/libnetcdfd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/NETCDF/lib/netcdf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/NETCDF/lib/netcdf.lib




#win32:CONFIG(release, debug|release): LIBS += -LD:/Develope/OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world341
#else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Develope/OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world341d

#INCLUDEPATH += D:/Develope/OpenCV/opencv/build/include \
#               D:/Develope/OpenCV/opencv/build/include/opencv \
#               D:/Develope/OpenCV/opencv/build/include/opencv2
#DEPENDPATH += D:/Develope/OpenCV/opencv/build/include \
#               D:/Develope/OpenCV/opencv/build/include/opencv \
#               D:/Develope/OpenCV/opencv/build/include/opencv2

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/opencv/build/x64/vc14/lib/libopencv_world341.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/opencv/build/x64/vc14/lib/libopencv_world341d.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/opencv/build/x64/vc14/lib/opencv_world341.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/opencv/build/x64/vc14/lib/opencv_world341d.lib

win32:CONFIG(release, debug|release): LIBS += -LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_core341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_aruco341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_bgsegm341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_bioinspired341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_calib3d341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_ccalib341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_core341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_datasets341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_dnn341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_dnn_objdetect341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_dpm341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_face341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_features2d341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_flann341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_fuzzy341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_hfs341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_highgui341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_imgcodecs341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_imgproc341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_img_hash341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_line_descriptor341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_ml341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_objdetect341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_optflow341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_phase_unwrapping341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_photo341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_plot341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_reg341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_rgbd341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_saliency341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_shape341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_stereo341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_stitching341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_structured_light341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_superres341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_surface_matching341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_text341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_tracking341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_video341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_videoio341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_videostab341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_xfeatures2d341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_ximgproc341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_xobjdetect341 \
-LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_xphoto341
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/ -lopencv_core341d

INCLUDEPATH += D:/Develope/OpenCV/vs2015_x32/include/opencv2 \
                D:/Develope/OpenCV/vs2015_x32/include/opencv \
                D:/Develope/OpenCV/vs2015_x32/include
DEPENDPATH += D:/Develope/OpenCV/vs2015_x32/include/opencv2 \
                D:/Develope/OpenCV/vs2015_x32/include/opencv \
                D:/Develope/OpenCV/vs2015_x32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/libopencv_core341.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/libopencv_core341d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_core341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_aruco341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_bgsegm341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_bioinspired341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_calib3d341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_ccalib341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_datasets341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_dnn341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_dnn_objdetect341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_dpm341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_face341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_features2d341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_flann341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_fuzzy341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_hfs341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_highgui341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_imgcodecs341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_imgproc341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_img_hash341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_line_descriptor341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_ml341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_objdetect341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_optflow341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_phase_unwrapping341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_photo341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_plot341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_reg341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_rgbd341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_saliency341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_shape341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_stereo341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_stitching341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_structured_light341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_superres341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_surface_matching341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_text341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_tracking341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_video341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_videoio341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_videostab341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_xfeatures2d341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_ximgproc341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_xobjdetect341.lib \
                                                                    D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_xphoto341.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/Develope/OpenCV/vs2015_x32/x86/vc14/lib/opencv_core341d.lib
