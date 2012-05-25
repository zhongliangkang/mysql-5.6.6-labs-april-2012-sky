/* Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef RPL_INFO_FILE_H
#define RPL_INFO_FILE_H

#include <my_global.h>
#include <sql_priv.h>
#include "rpl_info_handler.h"

class Rpl_info_factory;

/**
  Defines a file hander.
*/
class Rpl_info_file : public Rpl_info_handler
{
  friend class Rpl_info_factory;

public:
  virtual ~Rpl_info_file() { };

private:
  char info_fname[FN_REFLEN + 128];

  /*
    info_fd - file descriptor of the info file. set only during
    initialization or clean up - safe to read anytime
  */
  File info_fd;

  /* IO_CACHE of the info file - set only during init or end */
  IO_CACHE info_file;

  int do_init_info(const ulong *uidx, const uint nidx);
  enum_return_check do_check_info(const ulong *uidx, const uint nidx);
  void do_end_info(const ulong *uidx, const uint nidx);
  int do_flush_info(const ulong *uidx, const uint nidx,
                    const bool force);
  int do_remove_info(const ulong *uidx, const uint nidx);

  int do_prepare_info_for_read(const uint nidx);
  int do_prepare_info_for_write(const uint nidx);
  bool do_set_info(const int pos, const char *value);
  bool do_set_info(const int pos, const uchar *value,
                   const size_t size);
  bool do_set_info(const int pos, const int value);
  bool do_set_info(const int pos, const ulong value);
  bool do_set_info(const int pos, const float value);
  bool do_set_info(const int pos, const Dynamic_ids *value);
  bool do_get_info(const int pos, char *value, const size_t size,
                   const char *default_value);
  bool do_get_info(const int pos, uchar *value, const size_t size,
                   const uchar *default_value);
  bool do_get_info(const int pos, int *value,
                   const int default_value);
  bool do_get_info(const int pos, ulong *value,
                   const ulong default_value);
  bool do_get_info(const int pos, float *value,
                   const float default_value);
  bool do_get_info(const int pos, Dynamic_ids *value,
                   const Dynamic_ids *default_value);
  char* do_get_description_info();
  bool do_is_transactional();
  bool do_update_is_transactional();

  Rpl_info_file(int const nparam, const char* param_info_fname);
  Rpl_info_file(const Rpl_info_file& info);

  Rpl_info_file& operator=(const Rpl_info_file& info);
};
#endif /* RPL_INFO_FILE_H */
