add_definitions(-DTRANSLATION_DOMAIN=\"freespacenotifier\")

set(kded_freespacenotifier_SRCS freespacenotifier.cpp module.cpp)

ki18n_wrap_ui(kded_freespacenotifier_SRCS freespacenotifier_prefs_base.ui)

qt_add_dbus_interface(kded_freespacenotifier_SRCS ${KDED_DBUS_INTERFACE} kded_interface)

kconfig_add_kcfg_files(kded_freespacenotifier_SRCS settings.kcfgc)

kcoreaddons_add_plugin(freespacenotifier SOURCES ${kded_freespacenotifier_SRCS} INSTALL_NAMESPACE "kf5/kded")
kcoreaddons_desktop_to_json(freespacenotifier freespacenotifier.desktop)

target_link_libraries(freespacenotifier
    KF5::ConfigWidgets
    KF5::DBusAddons
    KF5::I18n
    KF5::KIOCore
    KF5::KIOGui
    KF5::Notifications
    KF5::Service
)

########### install files ###############

install( FILES freespacenotifier.notifyrc  DESTINATION  ${KDE_INSTALL_KNOTIFY5RCDIR} )
install( FILES freespacenotifier.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
