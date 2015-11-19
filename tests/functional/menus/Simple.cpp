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

#include <glib-object.h>
#include <gio/gio.h>

#include <memory>

using namespace std;
using namespace unity::gmenuharness;

pair<shared_ptr<GMenu>, shared_ptr<GSimpleActionGroup>>
createMenu()
{
    // Main menu
    shared_ptr<GMenu> menu(g_menu_new(), &g_object_deleter);

    // Actions
    shared_ptr<GSimpleActionGroup> ag(g_simple_action_group_new(), &g_object_deleter);

    // Submenu
    {
        shared_ptr<GMenu> submenu(g_menu_new(), &g_object_deleter);
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("First", "app.first"), &g_object_deleter);
            g_menu_item_set_attribute_value(item.get(), "description", g_variant_new_string("First description"));
            g_menu_append_item(submenu.get(), item.get());

            shared_ptr<GSimpleAction> action(g_simple_action_new("first", NULL),
                                                      &g_object_deleter);
            g_simple_action_set_enabled(action.get(), FALSE);
            g_action_map_add_action(G_ACTION_MAP(ag.get()), G_ACTION(action.get()));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Second", "app.second"), &g_object_deleter);
            g_menu_item_set_attribute_value(item.get(), "description", g_variant_new_string("Second description"));
            g_menu_append_item(submenu.get(), item.get());

            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("second", NULL)));
        }

        shared_ptr<GMenuItem> item(g_menu_item_new_submenu("Main", G_MENU_MODEL(submenu.get())), &g_object_deleter);
        g_menu_append_item(menu.get(), item.get());
    }

    return make_pair(menu, ag);
}
