/*****************************************************************************

Copyright (c) 2007, 2012, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

/**************************************************//**
@file handler/i_s.h
InnoDB INFORMATION SCHEMA tables interface to MySQL.

Created July 18, 2007 Vasil Dimov
*******************************************************/

#ifndef i_s_h
#define i_s_h

const char plugin_author[] = "Oracle Corporation";

extern struct st_mysql_plugin	i_s_innodb_trx;
extern struct st_mysql_plugin	i_s_innodb_locks;
extern struct st_mysql_plugin	i_s_innodb_lock_waits;
extern struct st_mysql_plugin	i_s_innodb_cmp;
extern struct st_mysql_plugin	i_s_innodb_cmp_reset;
extern struct st_mysql_plugin	i_s_innodb_cmpmem;
extern struct st_mysql_plugin	i_s_innodb_cmpmem_reset;
extern struct st_mysql_plugin   i_s_innodb_metrics;
extern struct st_mysql_plugin	i_s_innodb_ft_default_stopword;
extern struct st_mysql_plugin	i_s_innodb_ft_inserted;
extern struct st_mysql_plugin	i_s_innodb_ft_deleted;
extern struct st_mysql_plugin	i_s_innodb_ft_being_deleted;
extern struct st_mysql_plugin	i_s_innodb_ft_index_cache;
extern struct st_mysql_plugin	i_s_innodb_ft_index_table;
extern struct st_mysql_plugin	i_s_innodb_ft_config;
extern struct st_mysql_plugin	i_s_innodb_buffer_page;
extern struct st_mysql_plugin	i_s_innodb_buffer_page_lru;
extern struct st_mysql_plugin	i_s_innodb_buffer_stats;
extern struct st_mysql_plugin	i_s_innodb_sys_tables;
extern struct st_mysql_plugin	i_s_innodb_sys_tablestats;
extern struct st_mysql_plugin	i_s_innodb_sys_indexes;
extern struct st_mysql_plugin	i_s_innodb_sys_columns;
extern struct st_mysql_plugin	i_s_innodb_sys_fields;
extern struct st_mysql_plugin	i_s_innodb_sys_foreign;
extern struct st_mysql_plugin	i_s_innodb_sys_foreign_cols;
extern struct st_mysql_plugin	i_s_innodb_sys_tablespaces;
extern struct st_mysql_plugin	i_s_innodb_sys_datafiles;

#endif /* i_s_h */
