/*****************************************************************************

Copyright (c) 2009, 2011, Oracle and/or its affiliates. All Rights Reserved.

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
@file include/dict0stats.h
Code used for calculating and manipulating table statistics.

Created Jan 06, 2010 Vasil Dimov
*******************************************************/

#ifndef dict0stats_h
#define dict0stats_h

#include "univ.i"

#include "db0err.h"
#include "dict0types.h"
#include "trx0types.h"

enum dict_stats_upd_option {
	DICT_STATS_RECALC_PERSISTENT,/* (re) calculate the
				statistics using a precise and slow
				algo and save them to the persistent
				storage, if the persistent storage is
				not present then emit a warning and
				fall back to transient stats */
	DICT_STATS_RECALC_TRANSIENT,/* (re) calculate the statistics
				using an imprecise quick algo
				without saving the results
				persistently */
	DICT_STATS_EMPTY_TABLE,	/* Write all zeros (or 1 where it makes sense)
				into a table and its indexes' statistics
				members. The resulting stats correspond to an
				empty table. If the table is using persistent
				statistics, then they are saved on disk. */
	DICT_STATS_FETCH_ONLY_IF_NOT_IN_MEMORY /* fetch the stats
				from the persistent storage if the in-memory
				structures have not been initialized yet,
				otherwise do nothing */
};

typedef enum dict_stats_upd_option	dict_stats_upd_option_t;

/*********************************************************************//**
Set the persistent statistics flag for a given table. This is set only
in the in-memory table object and is not saved on disk. It will be read
from the .frm file upon first open from MySQL after a server restart. */
UNIV_INLINE
void
dict_stats_set_persistent(
/*======================*/
	dict_table_t*	table,	/*!< in/out: table */
	ibool		ps_on,	/*!< in: persistent stats explicitly enabled */
	ibool		ps_off)	/*!< in: persistent stats explicitly disabled */
	__attribute__((nonnull));

/*********************************************************************//**
Check whether persistent statistics is enabled for a given table.
@return TRUE if enabled, FALSE otherwise */
UNIV_INLINE
ibool
dict_stats_is_persistent_enabled(
/*=============================*/
	const dict_table_t*	table)	/*!< in: table */
	__attribute__((nonnull, warn_unused_result));

/*********************************************************************//**
Initialize table's stats for the first time when opening a table. */
UNIV_INLINE
void
dict_stats_init(
/*============*/
	dict_table_t*	table,	/*!< in/out: table */
	ibool		ps_on,	/*!< in: persistent stats explicitly enabled */
	ibool		ps_off,	/*!< in: persistent stats explicitly disabled */
	ibool		dict_locked)/*!< in: TRUE=data dictionary locked */
	__attribute__((nonnull));

/*********************************************************************//**
Deinitialize table's stats after the last close of the table. This is
used to detect "FLUSH TABLE" and refresh the stats upon next open. */
UNIV_INLINE
void
dict_stats_deinit(
/*==============*/
	dict_table_t*	table)	/*!< in/out: table */
	__attribute__((nonnull));

/*********************************************************************//**
Calculates new estimates for table and index statistics. The statistics
are used in query optimization.
@return DB_* error code or DB_SUCCESS */
UNIV_INTERN
enum db_err
dict_stats_update(
/*==============*/
	dict_table_t*		table,	/*!< in/out: table */
	dict_stats_upd_option_t	stats_upd_option,
					/*!< in: whether to (re) calc
					the stats or to fetch them from
					the persistent storage */
	ibool			caller_has_dict_sys_mutex);
					/*!< in: TRUE if the caller
					owns dict_sys->mutex */

/*********************************************************************//**
Removes the information for a particular index's stats from the persistent
storage if it exists and if there is data stored for this index.
The transaction is not committed, it must not be committed in this
function because this is the user trx that is running DROP INDEX.
The transaction will be committed at the very end when dropping an
index.
@return DB_SUCCESS or error code */
UNIV_INTERN
enum db_err
dict_stats_delete_index_stats(
/*==========================*/
	const char*	tname,	/*!< in: table name */
	const char*	iname,	/*!< in: index name */
	trx_t*		trx,	/*!< in/out: user transaction */
	char*		errstr, /*!< out: error message if != DB_SUCCESS
				is returned */
	ulint		errstr_sz);/*!< in: size of the errstr buffer */

/*********************************************************************//**
Removes the statistics for a table and all of its indexes from the
persistent storage if it exists and if there is data stored for the table.
This function creates its own transaction and commits it.
@return DB_SUCCESS or error code */
UNIV_INTERN
enum db_err
dict_stats_delete_table_stats(
/*==========================*/
	const char*	table_name,	/*!< in: table name */
	char*		errstr,		/*!< out: error message
					if != DB_SUCCESS is returned */
	ulint		errstr_sz);	/*!< in: size of errstr buffer */

/*********************************************************************//**
Fetches or calculates new estimates for index statistics. */
UNIV_INTERN
void
dict_stats_update_for_index(
/*========================*/
	dict_index_t*	index)	/*!< in/out: index */
	__attribute__((nonnull));

#ifndef UNIV_NONINL
#include "dict0stats.ic"
#endif

#endif /* dict0stats_h */
