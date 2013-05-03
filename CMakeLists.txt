# Project Needs a name ofcourse
project(plasma-ydpd)

# Define PLASMA macro
add_definitions(-DPLASMA)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(ydpd_SRCS plasma-ydpd.cpp
   config.cpp
   configtabdialog.cpp
   dictinstance.cpp
   configmanager.cpp
   dict.cpp
   dictmanager.cpp
   pathswidget.cpp
   ydpd.cpp
   )

# Now make sure all files get to the right place
qt4_add_resources(ydpd_RSCS Ydpd.qrc)
kde4_add_ui_files(ydpd_RSCS ydpdForm.ui startupForm.ui pathsForm.ui historyForm.ui)
kde4_add_plugin(plasma_applet_ydpd ${ydpd_SRCS} ${ydpd_RSCS})

target_link_libraries(plasma_applet_ydpd
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS}
		      ${KDE4_KIO_LIBS} ydpdict phonon)
 
install(TARGETS plasma_applet_ydpd
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-ydpd.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
