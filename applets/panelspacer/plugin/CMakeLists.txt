kde_enable_exceptions()

add_definitions(-DTRANSLATION_DOMAIN=\"panelspacer\")

kcoreaddons_add_plugin(org.kde.plasma.panelspacer SOURCES panelspacer.cpp INSTALL_NAMESPACE "plasma/applets")
kcoreaddons_desktop_to_json(org.kde.plasma.panelspacer ../package/metadata.desktop)

target_link_libraries(org.kde.plasma.panelspacer Qt::Gui Qt::Core Qt::Qml Qt::Quick KF5::Plasma KF5::PlasmaQuick KF5::I18n)
