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

#include <unity/gmenuharness/MatchResult.h>

#include <gtest/gtest.h>

using namespace std;
namespace mh = unity::gmenuharness;

namespace
{

class TestMatchResult : public testing::Test
{
};

TEST_F(TestMatchResult, SuccessFailAndMessage)
{
    mh::MatchResult matchResult;
    EXPECT_TRUE(matchResult.success());

    matchResult.failure({1, 2, 3}, "the message");
    matchResult.failure({1, 3, 4}, "the other message");
    EXPECT_FALSE(matchResult.success());
    EXPECT_EQ("Failed expectations:\n 1 2 3 the message\n 1 3 4 the other message\n", matchResult.concat_failures());
}

TEST_F(TestMatchResult, MergeTwoFailed)
{
    mh::MatchResult matchResult;
    matchResult.failure({1, 2, 3}, "m1a");
    matchResult.failure({1, 3, 4}, "m1b");

    mh::MatchResult matchResult2;
    matchResult2.failure({2, 2, 3}, "m2a");
    matchResult2.failure({2, 3, 4}, "m2b");
    EXPECT_FALSE(matchResult2.success());

    matchResult.merge(matchResult2);
    EXPECT_FALSE(matchResult.success());
    EXPECT_EQ("Failed expectations:\n 1 2 3 m1a\n 1 3 4 m1b\n 2 2 3 m2a\n 2 3 4 m2b\n", matchResult.concat_failures());
}

TEST_F(TestMatchResult, MergeFailedIntoSuccess)
{
    mh::MatchResult matchResult;
    EXPECT_TRUE(matchResult.success());

    mh::MatchResult matchResult2;
    matchResult2.failure({2, 2, 3}, "m2a");
    matchResult2.failure({2, 3, 4}, "m2b");
    EXPECT_FALSE(matchResult2.success());

    matchResult.merge(matchResult2);
    EXPECT_FALSE(matchResult.success());
    EXPECT_EQ("Failed expectations:\n 2 2 3 m2a\n 2 3 4 m2b\n", matchResult.concat_failures());
}

TEST_F(TestMatchResult, CopyAssignment)
{
    mh::MatchResult matchResult;
    matchResult.failure({1, 2, 3}, "m1a");
    matchResult.failure({1, 3, 4}, "m1b");

    // Copy constructor
    {
        mh::MatchResult matchResult2(matchResult);
        EXPECT_EQ("Failed expectations:\n 1 2 3 m1a\n 1 3 4 m1b\n", matchResult2.concat_failures());
    }
    // Assignment operator
    {
        mh::MatchResult matchResult2 = matchResult;
        EXPECT_EQ("Failed expectations:\n 1 2 3 m1a\n 1 3 4 m1b\n", matchResult2.concat_failures());
    }
    // Move operator
    {
        mh::MatchResult matchResult2 = move(matchResult);
        EXPECT_EQ("Failed expectations:\n 1 2 3 m1a\n 1 3 4 m1b\n", matchResult2.concat_failures());
    }
}

}
