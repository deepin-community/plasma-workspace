# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_notifications\")

set(kcm_notifications_SRCS
    kcm.cpp
    sourcesmodel.cpp
    filterproxymodel.cpp
    notificationsdata.cpp
)

kcoreaddons_add_plugin(kcm_notifications SOURCES ${kcm_notifications_SRCS} INSTALL_NAMESPACE "kcms")
target_link_libraries(kcm_notifications
    KF5::KCMUtils
    KF5::CoreAddons
    KF5::Declarative
    KF5::GlobalAccel
    KF5::GuiAddons
    KF5::I18n
    KF5::QuickAddons
    KF5::NotifyConfig
    KF5::Service
    PW::LibNotificationManager
)

kcoreaddons_desktop_to_json(kcm_notifications "kcm_notifications.desktop")

install(FILES kcm_notifications.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

kpackage_install_package(package kcm_notifications kcms)

