/* Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02111-1307  USA */

// Always include (the generated) my_config.h, to get correct platform defines.
#include "my_config.h"
#include <gtest/gtest.h>

#include "test_utils.h"
#include "my_stacktrace.h"
#include "m_string.h"

namespace {

using my_testing::Server_initializer;
using my_testing::Mock_error_handler;

class FatalSignalDeathTest : public ::testing::Test
{
protected:
  static void SetUpTestCase()
  { Server_initializer::SetUpTestCase(); }

  static void TearDownTestCase()
  { Server_initializer::TearDownTestCase(); }

  virtual void SetUp()
  {
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    initializer.SetUp();
  }

  virtual void TearDown()
  { initializer.TearDown(); }

  Server_initializer initializer;
};


TEST_F(FatalSignalDeathTest, Abort)
{
#if defined(__WIN__)
  EXPECT_DEATH_IF_SUPPORTED(abort(), ".* UTC - mysqld got exception.*");
#else
  EXPECT_DEATH_IF_SUPPORTED(abort(), ".* UTC - mysqld got signal 6.*");
#endif
}


TEST_F(FatalSignalDeathTest, Segfault)
{
  int *pint= NULL;
#if defined(__WIN__)
  EXPECT_DEATH_IF_SUPPORTED(*pint= 42, ".* UTC - mysqld got exception.*");
#else
  // On most platforms we get SIGSEGV == 11, but SIGBUS == 10 is also possible.
  EXPECT_DEATH_IF_SUPPORTED(*pint= 42, ".* UTC - mysqld got signal 1.*");
#endif
}


// A simple helper function to determine array size.
template <class T, int size>
int array_size(const T (&)[size])
{
  return size;
}


TEST(PrintUtilities, Utoa)
{
  char buff[22];
  ulonglong intarr[]= { 0, 1, 8, 12, 1234, 88888, ULONG_MAX, ULONGLONG_MAX };
  char sprintbuff[22];
  for (int ix= 0; ix < array_size(intarr); ++ix)
  {
    char *my_res;
    sprintf(sprintbuff, "%llu", intarr[ix]);
    my_res= my_safe_utoa(10, intarr[ix], &buff[sizeof(buff)-1]);
    EXPECT_STREQ(sprintbuff, my_res);

    if (intarr[ix] <= ULONG_MAX)
    {
      sprintf(sprintbuff, "%lu", (ulong) intarr[ix]);
      my_res= my_safe_utoa(10, (ulong) intarr[ix], &buff[sizeof(buff)-1]);
      EXPECT_STREQ(sprintbuff, my_res);
    }
  }
}


TEST(PrintUtilities, Itoa)
{
  char buff[22];
  char sprintbuff[22];
  longlong intarr[]= { 0, 1, 8, 12, 1234, 88888, LONG_MAX, LONGLONG_MAX };

  for (int ix= 0; ix < array_size(intarr); ++ix)
  {
    char *my_res;
    sprintf(sprintbuff, "%lld", intarr[ix]);
    my_res= my_safe_itoa(10, intarr[ix], &buff[sizeof(buff)-1]);
    EXPECT_STREQ(sprintbuff, my_res);

    ll2str(intarr[ix], buff, 10, 0);
    EXPECT_STREQ(sprintbuff, buff);

    sprintf(sprintbuff, "%lld", -intarr[ix]);
    my_res= my_safe_itoa(10, -intarr[ix], &buff[sizeof(buff)-1]);
    EXPECT_STREQ(sprintbuff, my_res);

    // This one fails ....
    // ll2str(-intarr[ix], buff, 10, 0);
    // EXPECT_STREQ(sprintbuff, buff)
    //  << "failed for " << -intarr[ix];

    sprintf(sprintbuff, "%llx", intarr[ix]);
    my_res= my_safe_itoa(16, intarr[ix], &buff[sizeof(buff)-1]);
    EXPECT_STREQ(sprintbuff, my_res);

    ll2str(intarr[ix], buff, 16, 0);
    EXPECT_STREQ(sprintbuff, buff);

    sprintf(sprintbuff, "%llx", -intarr[ix]);
    my_res= my_safe_itoa(16, -intarr[ix], &buff[sizeof(buff)-1]);
    EXPECT_STREQ(sprintbuff, my_res)
      << "failed for " << -intarr[ix];

    ll2str(-intarr[ix], buff, 16, 0);
    EXPECT_STREQ(sprintbuff, buff);
  }
}


TEST(PrintUtilities, Printf)
{
  char buff[512];
  char sprintfbuff[512];
  const char *null_str= NULL;

  my_safe_snprintf(buff, sizeof(buff), "hello");
  EXPECT_STREQ("hello", buff);
 
  my_safe_snprintf(buff, sizeof(buff), "hello %s hello", "hello");
  EXPECT_STREQ("hello hello hello", buff);
  my_safe_snprintf(buff, sizeof(buff), "hello %s hello", null_str);
  EXPECT_STREQ("hello (null) hello", buff);
 
  my_safe_snprintf(buff, sizeof(buff), "hello %d hello", 42);
  EXPECT_STREQ("hello 42 hello", buff);
  my_safe_snprintf(buff, sizeof(buff), "hello %i hello", 42);
  EXPECT_STREQ("hello 42 hello", buff);
  my_safe_snprintf(buff, sizeof(buff), "hello %u hello", (unsigned) 42);
  EXPECT_STREQ("hello 42 hello", buff);

  my_safe_snprintf(buff, sizeof(buff), "hello %llu hello", ULONGLONG_MAX);
  sprintf(sprintfbuff, "hello %llu hello", ULONGLONG_MAX);
  EXPECT_STREQ(sprintfbuff, buff);

  my_safe_snprintf(buff, sizeof(buff), "hello %x hello", 42);
  EXPECT_STREQ("hello 2a hello", buff);

  my_safe_snprintf(buff, sizeof(buff), "hello %x hello", -42);
  sprintf(sprintfbuff, "hello %x hello", -42);
  EXPECT_STREQ("hello ffffffd6 hello", buff);
  EXPECT_STREQ(sprintfbuff, buff);

  my_safe_snprintf(buff, sizeof(buff), "hello %llx hello", (longlong) -42);
  sprintf(sprintfbuff, "hello %llx hello", (longlong) -42);
  EXPECT_STREQ("hello ffffffffffffffd6 hello", buff);
  EXPECT_STREQ(sprintfbuff, buff);

  void *p= this;
  my_safe_snprintf(buff, sizeof(buff), "hello 0x%p hello", p);
  my_snprintf(sprintfbuff, sizeof(sprintfbuff), "hello %p hello", p);
  EXPECT_STREQ(sprintfbuff, buff);
}

}
