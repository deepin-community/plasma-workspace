add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.private.systemtray\")

plasma_install_package(package org.kde.plasma.private.systemtray)

include_directories(${plasma-workspace_SOURCE_DIR}/statusnotifierwatcher)

set(systemtray_SRCS
    dbusserviceobserver.cpp
    plasmoidregistry.cpp
    sortedsystemtraymodel.cpp
    statusnotifieritemjob.cpp
    statusnotifieritemhost.cpp
    statusnotifieritemservice.cpp
    statusnotifieritemsource.cpp
    systemtraymodel.cpp
    systemtraysettings.cpp
    systemtraytypes.cpp
)

qt_add_dbus_interface(systemtray_SRCS ${KNOTIFICATIONS_DBUS_INTERFACES_DIR}/kf5_org.kde.StatusNotifierWatcher.xml statusnotifierwatcher_interface)
qt_add_dbus_interface(systemtray_SRCS ${plasma-workspace_SOURCE_DIR}/dataengines/mpris2/org.freedesktop.DBus.Properties.xml dbusproperties)

set(statusnotifieritem_xml ${KNOTIFICATIONS_DBUS_INTERFACES_DIR}/kf5_org.kde.StatusNotifierItem.xml)
set_source_files_properties(${statusnotifieritem_xml} PROPERTIES
   NO_NAMESPACE false
   INCLUDE "systemtraytypes.h"
   CLASSNAME OrgKdeStatusNotifierItem
)
qt_add_dbus_interface(systemtray_SRCS ${statusnotifieritem_xml} statusnotifieritem_interface)

ecm_qt_declare_logging_category(systemtray_SRCS HEADER debug.h
                                            IDENTIFIER SYSTEM_TRAY
                                            CATEGORY_NAME kde.systemtray
                                            DEFAULT_SEVERITY Info)
add_library(systemtraymodel_static STATIC ${systemtray_SRCS})
target_link_libraries(systemtraymodel_static
    Qt::Gui
    Qt::Quick
    Qt::DBus
    KF5::XmlGui
    KF5::I18n
    KF5::ItemModels
    KF5::Plasma
    KF5::IconThemes
    KF5::WindowSystem
    dbusmenuqt)

kcoreaddons_add_plugin(org.kde.plasma.private.systemtray SOURCES systemtray.cpp INSTALL_NAMESPACE "plasma/applets")
kcoreaddons_desktop_to_json(org.kde.plasma.private.systemtray package/metadata.desktop)

target_link_libraries(org.kde.plasma.private.systemtray systemtraymodel_static)

add_subdirectory(container)
if(BUILD_TESTING)
  add_subdirectory(autotests)
  add_subdirectory(tests)
endif()
