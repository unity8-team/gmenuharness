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

    // File menu
    {
        shared_ptr<GMenu> fileMenu(g_menu_new(), &g_object_deleter);
        {
            shared_ptr<GMenu> newMenu(g_menu_new(), &g_object_deleter);
            {
                {
                    shared_ptr<GMenuItem> item(g_menu_item_new("Apple", "app.new-apple"), &g_object_deleter);
                    g_menu_item_set_attribute_value(
                            item.get(), "x-foo-pass-through-action",
                            g_variant_new_string("app.pass-through-action-string"));
                    g_menu_append_item(newMenu.get(), item.get());
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                G_ACTION(g_simple_action_new("new-apple", NULL)));

                    shared_ptr<GSimpleAction> passThroughAction(
                            g_simple_action_new_stateful(
                                    "pass-through-action-string", NULL,
                                    g_variant_new_string("string-value-passthrough")),
                            &g_object_deleter);
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                            G_ACTION(passThroughAction.get()));
                }
                {
                    shared_ptr<GMenuItem> item(g_menu_item_new("Banana", "app.new-banana"), &g_object_deleter);
                    g_menu_item_set_attribute_value(
                            item.get(), "x-foo-pass-through-action",
                            g_variant_new_string("app.pass-through-action-bool"));
                    g_menu_append_item(newMenu.get(), item.get());
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                G_ACTION(g_simple_action_new("new-banana", NULL)));

                    shared_ptr<GSimpleAction> passThroughAction(
                                        g_simple_action_new_stateful(
                                                "pass-through-action-bool", NULL,
                                                g_variant_new_boolean(TRUE)),
                                        &g_object_deleter);
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                            G_ACTION(passThroughAction.get()));
                }
                {
                    shared_ptr<GMenuItem> item(g_menu_item_new("Coconut", "app.new-coconut"), &g_object_deleter);
                    g_menu_item_set_attribute_value(
                            item.get(), "x-foo-pass-through-action",
                            g_variant_new_string("app.pass-through-action-double"));
                    g_menu_append_item(newMenu.get(), item.get());
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                G_ACTION(g_simple_action_new("new-coconut", NULL)));

                    shared_ptr<GSimpleAction> passThroughAction(
                                        g_simple_action_new_stateful(
                                                "pass-through-action-double", NULL,
                                                g_variant_new_double(3.14)),
                                        &g_object_deleter);
                    g_action_map_add_action(G_ACTION_MAP(ag.get()),
                                            G_ACTION(passThroughAction.get()));
                }

                shared_ptr<GMenuItem> item(g_menu_item_new_submenu("New", G_MENU_MODEL(newMenu.get())), &g_object_deleter);
                g_menu_append_item(fileMenu.get(), item.get());
            }
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Open", "app.open"), &g_object_deleter);
            g_menu_append_item(fileMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("open", NULL)));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Save", "app.save"), &g_object_deleter);
            g_menu_append_item(fileMenu.get(), item.get());
            GSimpleAction *action = G_SIMPLE_ACTION(g_simple_action_new("save", NULL));
            g_simple_action_set_enabled(action, FALSE);
            g_action_map_add_action(G_ACTION_MAP(ag.get()), G_ACTION(action));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Quit", "app.quit"), &g_object_deleter);
            g_menu_append_item(fileMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("quit", NULL)));
        }

        shared_ptr<GMenuItem> item(g_menu_item_new_submenu("File", G_MENU_MODEL(fileMenu.get())), &g_object_deleter);
        g_menu_append_item(menu.get(), item.get());
    }

    // Edit menu
    {
        shared_ptr<GMenu> editMenu(g_menu_new(), &g_object_deleter);
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Undo", "app.undo"), &g_object_deleter);
            g_menu_append_item(editMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("undo", NULL)));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Cut", "app.cut"), &g_object_deleter);
            g_menu_append_item(editMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("cut", NULL)));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Copy", "app.copy"), &g_object_deleter);
            g_menu_append_item(editMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("copy", NULL)));
        }
        {
            shared_ptr<GMenuItem> item(g_menu_item_new("Paste", "app.paste"), &g_object_deleter);
            g_menu_append_item(editMenu.get(), item.get());
            g_action_map_add_action(G_ACTION_MAP(ag.get()),
                        G_ACTION(g_simple_action_new("paste", NULL)));
        }

        shared_ptr<GMenuItem> item(g_menu_item_new_submenu("Edit", G_MENU_MODEL(editMenu.get())), &g_object_deleter);
        g_menu_append_item(menu.get(), item.get());
    }

    return make_pair(menu, ag);
}
