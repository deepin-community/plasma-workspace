#!/bin/sh
# SPDX-FileCopyrightText: 2019 Aleix Pol Gonzalez <aleixpol@kde.org>
# SPDX-FileCopyrightText: 2021 Nate Graham <nate@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later

set -e

# Make built-from-source sessions appear in login screen
sudo install -D @CMAKE_CURRENT_BINARY_DIR@/plasmax11-dev.desktop /usr/share/xsessions/
sudo install -D @CMAKE_CURRENT_BINARY_DIR@/plasmawayland-dev.desktop /usr/share/wayland-sessions/
install @CMAKE_BINARY_DIR@/prefix.sh @CMAKE_INSTALL_FULL_LIBEXECDIR@/plasma-dev-prefix.sh
install @CMAKE_CURRENT_BINARY_DIR@/startplasma-dev.sh @CMAKE_INSTALL_FULL_LIBEXECDIR@

# Make the system DBus able to see any new DBus files that have been added to
# the built-from-source plasma session which are not yet present in the system
# DBus locations. Because some distros have security policies which prevent the
# use of DBus files in a user's homedir, and even symlinks outside,
# we have to copy the files into a system-owned location.
sudo mkdir -p /opt/kde-dbus-scripts/
sudo cp -r @KDE_INSTALL_FULL_DBUSDIR@/* /opt/kde-dbus-scripts/

old_file='/etc/dbus-1/session-local.conf'
[ -e $old_file ] && grep --quiet '/opt/kde-dbus-scripts' $old_file && sudo rm -v $old_file

cat > 00-plasma.conf << EOF
<!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-Bus Bus Configuration 1.0//EN"
 "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
<!-- This file is autogenerated by Plasma's install-sessions.sh; Changes may get overwritten! -->
<busconfig>
    <servicedir>/opt/kde-dbus-scripts/services</servicedir>
    <includedir>/opt/kde-dbus-scripts/session.d/</includedir>
    <standard_session_servicedirs/>
</busconfig>
EOF
sudo mkdir -p /etc/dbus-1/session.d/
sudo mv 00-plasma.conf /etc/dbus-1/session.d/

