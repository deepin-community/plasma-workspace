# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_desktoptheme\")

set(kcm_desktoptheme_SRCS
  kcm.cpp
  themesmodel.cpp
  filterproxymodel.cpp
)

kcmutils_generate_module_data(
    kcm_desktoptheme_SRCS
    MODULE_DATA_HEADER desktopthemedata.h
    MODULE_DATA_CLASS_NAME DesktopThemeData
    SETTINGS_HEADERS desktopthemesettings.h
    SETTINGS_CLASSES DesktopThemeSettings
)

kconfig_add_kcfg_files(kcm_desktoptheme_SRCS desktopthemesettings.kcfgc GENERATE_MOC)

kcoreaddons_add_plugin(kcm_desktoptheme SOURCES ${kcm_desktoptheme_SRCS} INSTALL_NAMESPACE "kcms")

target_link_libraries(kcm_desktoptheme
  KF5::CoreAddons
  KF5::KCMUtils
  KF5::KIOCore
  KF5::KIOWidgets
  KF5::I18n
  KF5::Plasma
  KF5::Declarative
  KF5::QuickAddons
)

kcoreaddons_desktop_to_json(kcm_desktoptheme "kcm_desktoptheme.desktop" SERVICE_TYPES kcmodule.desktop)

set(plasma-apply-desktoptheme_SRCS
    plasma-apply-desktoptheme.cpp
    themesmodel.cpp
)

add_executable(plasma-apply-desktoptheme ${plasma-apply-desktoptheme_SRCS})

target_link_libraries(plasma-apply-desktoptheme
  KF5::CoreAddons
  KF5::KCMUtils
  KF5::I18n
  KF5::Plasma
)

#this desktop file is installed only for retrocompatibility with sycoca
install(FILES kcm_desktoptheme.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES plasma-themes.knsrc DESTINATION ${KDE_INSTALL_KNSRCDIR})
install(TARGETS plasma-apply-desktoptheme DESTINATION ${KDE_INSTALL_BINDIR})

kpackage_install_package(package kcm_desktoptheme kcms)
