/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "remoteconfigs.h"
#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("TuxR");
    app.setApplicationName("TuxR");
    app.setApplicationVersion("0.0.1");
    app.setWindowIcon(QIcon::fromTheme("tuxr"));

    QScopedPointer<RemoteConfigs> remotes(RemoteConfigs::instance());
    QScopedPointer<Settings> settings(Settings::instance());

    MainWindow window;
    window.show();

    return app.exec();
}
