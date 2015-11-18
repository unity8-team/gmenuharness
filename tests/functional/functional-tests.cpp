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

#include <unity/gmenuharness/MenuMatcher.h>

#include <gtest/gtest.h>

#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbustest/QProcessDBusService.h>

#include <QDBusConnection>
#include <QString>

using namespace std;
using namespace QtDBusTest;
namespace mh = unity::gmenuharness;

namespace
{

static const char* DEFAULT_NAME {"default.name"};
static const char* DEFAULT_MENU_PATH {"/default/menu/path"};
static const char* DEFAULT_ACTIONS_PATH {"/default/actions/path"};

class FunctionalTests : public testing::Test
{
protected:
    void addMenu(const QString& menu, const QString& dbusName = DEFAULT_NAME,
            const QString& menuPath = DEFAULT_MENU_PATH,
            const QString& actionsPath = DEFAULT_ACTIONS_PATH)
    {
        dbus.registerService(
                DBusServicePtr(
                        new QProcessDBusService(dbusName,
                                                QDBusConnection::SessionBus,
                                                MENU_DIR "/" + menu,
                                                { dbusName, menuPath, actionsPath })));
    }

    void start()
    {
        dbus.startServices();
    }

    mh::MenuMatcher::Parameters parameters(const string& dbusName = DEFAULT_NAME,
                                           const string& menuPath = DEFAULT_MENU_PATH,
                                           const string& actionsPath = DEFAULT_ACTIONS_PATH)
    {
        return mh::MenuMatcher::Parameters(dbusName, {{ "app", actionsPath }},
                                           menuPath);
    }

    DBusTestRunner dbus;
};

TEST_F(FunctionalTests, ImportSimple)
{
    addMenu("simple");
    ASSERT_NO_THROW(start());

    EXPECT_MATCHRESULT(mh::MenuMatcher(parameters())
        .item(mh::MenuItemMatcher()
            .label("Main")
            .mode(mh::MenuItemMatcher::Mode::all)
            .submenu()
            .item(mh::MenuItemMatcher()
                .submenu()
                .label("First")
                .string_attribute("description", "First description")
                .action("app.first")
            )
            .item(mh::MenuItemMatcher()
                .submenu()
                .label("Second")
                .string_attribute("description", "Second description")
                .action("app.second")
            )
        ).match());
}

TEST_F(FunctionalTests, ImportDeeperMatchAll)
{
    addMenu("deeper");
    ASSERT_NO_THROW(start());

    EXPECT_MATCHRESULT(mh::MenuMatcher(parameters())
        .item(mh::MenuItemMatcher()
            .label("File")
            .mode(mh::MenuItemMatcher::Mode::all)
            .submenu()
            .item(mh::MenuItemMatcher()
                .submenu()
                .label("New")
                .mode(mh::MenuItemMatcher::Mode::all)
                .item(mh::MenuItemMatcher()
                    .label("Apple")
                    .action("app.new-apple")
                    .pass_through_string_attribute("x-foo-pass-through-action", "string-value-passthrough")
                )
                .item(mh::MenuItemMatcher()
                    .label("Banana")
                    .action("app.new-banana")
                    .pass_through_boolean_attribute("x-foo-pass-through-action", true)
                )
                .item(mh::MenuItemMatcher()
                    .label("Coconut")
                    .action("app.new-coconut")
                    .pass_through_double_attribute("x-foo-pass-through-action", 3.14)
                )
            )
            .item(mh::MenuItemMatcher()
                .label("Open")
                .action("app.open")
            )
            .item(mh::MenuItemMatcher()
                .label("Save")
                .action("app.save")
            )
            .item(mh::MenuItemMatcher()
                .label("Quit")
                .action("app.quit")
            )
        )
        .item(mh::MenuItemMatcher()
            .label("Edit")
            .mode(mh::MenuItemMatcher::Mode::all)
            .submenu()
            .item(mh::MenuItemMatcher()
                .label("Undo")
                .action("app.undo")
            )
            .item(mh::MenuItemMatcher()
                .label("Cut")
                .action("app.cut")
            )
            .item(mh::MenuItemMatcher()
                .label("Copy")
                .action("app.copy")
            )
            .item(mh::MenuItemMatcher()
                .label("Paste")
                .action("app.paste")
            )
        ).match());
}

}
