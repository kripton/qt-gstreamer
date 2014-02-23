set(QtGStreamer_SRCS
    init.cpp
    objectstore.cpp
    value.cpp
    structure.cpp
    caps.cpp
    miniobject.cpp
    object.cpp
    pad.cpp
    ghostpad.cpp
    element.cpp
    pluginfeature.cpp
    elementfactory.cpp
    bin.cpp
    childproxy.cpp
    pipeline.cpp
    message.cpp
    bus.cpp
    parse.cpp
    urihandler.cpp
    videoorientation.cpp
    videooverlay.cpp
    streamvolume.cpp
    colorbalance.cpp
    query.cpp
    clock.cpp
    memory.cpp
    buffer.cpp
    event.cpp
    clocktime.cpp
    taglist.cpp
    sample.cpp
    bufferlist.cpp
    discoverer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/gen.cpp
)

set(QtGStreamerUi_SRCS
    Ui/videowidget.cpp
    Ui/graphicsvideosurface.cpp
    Ui/graphicsvideowidget.cpp
)

set(QtGStreamerUtils_SRCS
    Utils/applicationsink.cpp
    Utils/applicationsource.cpp
)

set(QtGStreamer_INSTALLED_HEADERS
    global.h            Global
    init.h              Init
    enums.h
    structs.h           Fourcc
                        Fraction
                        IntRange
                        Int64Range
                        DoubleRange
                        FractionRange
    structure.h         Structure
    caps.h              Caps
    miniobject.h        MiniObject
    object.h            Object
    pad.h               Pad
    ghostpad.h          GhostPad
    element.h           Element
    pluginfeature.h     PluginFeature
    elementfactory.h    ElementFactory
    bin.h               Bin
    childproxy.h        ChildProxy
    pipeline.h          Pipeline
    message.h           Message
    bus.h               Bus
    parse.h             Parse
    urihandler.h        UriHandler
    videoorientation.h  VideoOrientation
    videooverlay.h      VideoOverlay
    streamvolume.h      StreamVolume
    colorbalance.h      ColorBalance
    query.h             Query
    clock.h             Clock
    buffer.h            Buffer
    sample.h            Sample
    memory.h            Memory
    event.h             Event
    clocktime.h         ClockTime
    taglist.h           TagList
    bufferlist.h        BufferList
    discoverer.h        Discoverer

    Ui/global.h
    Ui/videowidget.h            Ui/VideoWidget
    Ui/graphicsvideosurface.h   Ui/GraphicsVideoSurface
    Ui/graphicsvideowidget.h    Ui/GraphicsVideoWidget

    Utils/global.h
    Utils/applicationsink.h     Utils/ApplicationSink
    Utils/applicationsource.h   Utils/ApplicationSource
)

file(GLOB QtGStreamer_CODEGEN_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
list(REMOVE_ITEM QtGStreamer_CODEGEN_HEADERS "global.h")

set(QtGStreamer_CODEGEN_INCLUDES
    -Igst/gst.h
    -Igst/audio/audio-enumtypes.h
    -Igst/audio/streamvolume.h
    -Igst/video/video-enumtypes.h
    -Igst/video/videooverlay.h
    -Igst/video/colorbalance.h
    -Igst/video/videoorientation.h
    -Igst/app/gstappsrc.h
    -Igst/pbutils/gstdiscoverer.h
    -Igst/pbutils/pbutils-enumtypes.h
    -IQGlib/Quark
)


# Setup the environment
set(QTGSTREAMER_API_VERSION 1.0)
set(QTGSTREAMER_SOVERSION 0)
set(QTGSTREAMER_UI_SOVERSION 0)
set(QTGSTREAMER_UTILS_SOVERSION 0)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GSTREAMER_INCLUDE_DIR}
    ${GSTREAMER_AUDIO_INCLUDE_DIR}
    ${GSTREAMER_VIDEO_INCLUDE_DIR}
    ${GSTREAMER_BASE_INCLUDE_DIR}
    ${GSTREAMER_APP_INCLUDE_DIR}
    ${GSTREAMER_PBUTILS_INCLUDE_DIR}
    ${GLIB2_INCLUDE_DIR}
)
add_definitions(-DGST_DISABLE_XML -DGST_DISABLE_LOADSAVE)

if (QT_QTOPENGL_FOUND AND (OPENGL_FOUND OR OPENGLES2_FOUND))
    set(QtGStreamerUi_GL_LIBS ${QT_QTOPENGL_LIBRARY})
    if (OPENGLES2_FOUND)
        include_directories(${OPENGLES2_INCLUDE_DIR})
    else()
        include_directories(${OPENGL_INCLUDE_DIR})
    endif()
else()
    add_definitions(-DQTGSTREAMER_UI_NO_OPENGL)
endif()

# Add command to generate gen.cpp using codegen
run_codegen("QGst" "${QtGStreamer_CODEGEN_INCLUDES}" "${QtGStreamer_CODEGEN_HEADERS}")

# Build and link QtGStreamer
automoc4_add_library(QtGStreamer ${SHARED_OR_STATIC} ${QtGStreamer_SRCS})
set_target_properties(QtGStreamer PROPERTIES OUTPUT_NAME QtGStreamer-${QTGSTREAMER_API_VERSION}
                                               SOVERSION ${QTGSTREAMER_SOVERSION}
                                                 VERSION ${QTGSTREAMER_VERSION})
target_link_libraries(QtGStreamer PRIVATE
                                  ${QTGLIB_LIBRARY} ${GOBJECT_LIBRARIES}
                                  ${GSTREAMER_LIBRARY}
                                  ${GSTREAMER_AUDIO_LIBRARY}
                                  ${GSTREAMER_VIDEO_LIBRARY}
                                  ${GSTREAMER_PBUTILS_LIBRARY})
target_link_libraries(QtGStreamer PUBLIC ${QTGLIB_LIBRARY})

# Build and link QtGStreamerUi
automoc4_add_library(QtGStreamerUi ${SHARED_OR_STATIC} ${QtGStreamerUi_SRCS})
set_target_properties(QtGStreamerUi PROPERTIES OUTPUT_NAME QtGStreamerUi-${QTGSTREAMER_API_VERSION}
                                                 SOVERSION ${QTGSTREAMER_UI_SOVERSION}
                                                   VERSION ${QTGSTREAMER_VERSION})
target_link_libraries(QtGStreamerUi PRIVATE ${QT_QTGUI_LIBRARY} ${QTGSTREAMER_LIBRARY} ${QtGStreamerUi_GL_LIBS})
target_link_libraries(QtGStreamerUi PUBLIC ${QT_QTGUI_LIBRARY} ${QTGSTREAMER_LIBRARY})

# Build and link QtGStreamerUtils
automoc4_add_library(QtGStreamerUtils ${SHARED_OR_STATIC} ${QtGStreamerUtils_SRCS})
set_target_properties(QtGStreamerUtils PROPERTIES OUTPUT_NAME QtGStreamerUtils-${QTGSTREAMER_API_VERSION}
                                                    SOVERSION ${QTGSTREAMER_UTILS_SOVERSION}
                                                      VERSION ${QTGSTREAMER_VERSION})
target_link_libraries(QtGStreamerUtils PRIVATE ${QTGSTREAMER_LIBRARY} ${GSTREAMER_LIBRARY} ${GSTREAMER_APP_LIBRARY})
target_link_libraries(QtGStreamerUtils PUBLIC ${QTGSTREAMER_LIBRARY})

# Install
install(TARGETS QtGStreamer DESTINATION ${LIB_INSTALL_DIR} EXPORT ${EXPORT_TARGET_SET})
install(TARGETS QtGStreamerUi DESTINATION ${LIB_INSTALL_DIR} EXPORT ${EXPORT_TARGET_SET})
install(TARGETS QtGStreamerUtils DESTINATION ${LIB_INSTALL_DIR} EXPORT ${EXPORT_TARGET_SET})
install_headers("QGst" ${QtGStreamer_INSTALLED_HEADERS})
