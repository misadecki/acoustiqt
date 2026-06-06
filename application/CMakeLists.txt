set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW REQUIRED fftw3)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/acoustiqt.desktop.in
  ${CMAKE_CURRENT_BINARY_DIR}/acoustiqt.desktop
  @ONLY
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/acoustiqt.desktop
  DESTINATION share/applications
)

install(
  FILES resources/imgs/acoustiqt.svg
  DESTINATION share/icons/hicolor/scalable/apps
)

qt_add_executable(application
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/udp_receiver.cpp
    src/spectrogram_visualizer.cpp
    src/spectrum_visualizer.cpp
    src/fft_processor.cpp
    src/mainwindow.ui
    src/spectrogram_scale_widget.cpp
    src/axis_painter.cpp
    src/influx_client.cpp

    resources/resources.qrc

    inc/mainwindow.hh
    inc/axis_painter.hh
    inc/udp_receiver.hh
    inc/spectrogram_visualizer.hh
    inc/spectrum_visualizer.hh
    inc/fft_processor.hh
    inc/spectrogram_scale_widget.hh
    inc/influx_client.hh

    ../shared/protocol.hh
    ../shared/protocol.cpp
    ../shared/audio_config.hh
)

target_include_directories(application PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/../shared"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${FFTW_INCLUDE_DIRS}
)

target_link_libraries(application
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Svg
        ${FFTW_LIBRARIES}
)

target_compile_options(application PRIVATE
    -Wall
    -Wextra
    -pedantic
    -Werror=return-type
    -Werror=parentheses
    -Werror=vla
    -Werror=array-bounds
    -Wcast-qual
)

install(TARGETS application
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET application
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

