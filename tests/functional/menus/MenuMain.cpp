/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <unity/gmenuharness/MatchUtils.h>
#include <unity/util/ResourcePtr.h>

#include <glib-object.h>
#include <gio/gio.h>
#include <glib-unix.h>

#include <iostream>
#include <memory>

using namespace std;
using namespace unity::util;
using namespace unity::gmenuharness;

static gboolean
onSignal(gpointer data)
{
    g_main_loop_quit((GMainLoop*) data);
    return G_SOURCE_REMOVE;
}

pair<shared_ptr<GMenu>, shared_ptr<GSimpleActionGroup>>
createMenu();

int
main(int argc, char** argv)
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " DBUS_NAME MENU_PATH ACTIONS_PATH" << endl;
        return 1;
    }

    auto menu = createMenu();

    shared_ptr<GDBusConnection> session(
            g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL), &g_object_deleter);

    ResourcePtr<guint, function<void(guint)>> actionGroupExport(
            g_dbus_connection_export_action_group(
                    session.get(), argv[3], G_ACTION_GROUP(menu.second.get()),
                    NULL),
            [session](guint id)
            {
                g_dbus_connection_unexport_action_group(session.get(), id);
            });

    ResourcePtr<guint, function<void(guint)>> menuExport(
            g_dbus_connection_export_menu_model(session.get(), argv[2],
                    G_MENU_MODEL(menu.first.get()),
                    NULL),
            [session](guint id)
            {
                g_dbus_connection_unexport_menu_model(session.get(), id);
            });

    ResourcePtr<guint, function<void(guint)>> ownName(
            g_bus_own_name(G_BUS_TYPE_SESSION, argv[1], G_BUS_NAME_OWNER_FLAGS_NONE,
                   NULL, NULL, NULL, NULL, NULL),
            [](guint id)
            {
                g_bus_unown_name(id);
            });

    shared_ptr<GMainLoop> mainloop(g_main_loop_new(NULL, FALSE),
                                   &g_main_loop_unref);

    g_unix_signal_add(SIGTERM, onSignal, mainloop.get());
    g_unix_signal_add(SIGHUP, onSignal, mainloop.get());
    g_unix_signal_add(SIGINT, onSignal, mainloop.get());

    g_main_loop_run(mainloop.get());

    return 0;
}
