/*****************************************************************************
 * snapshot.h : vout internal snapshot
 *****************************************************************************
 * Copyright (C) 2009 Laurent Aimar
 * $Id$
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#if defined(__PLUGIN__) || defined(__BUILTIN__) || !defined(__LIBVLC__)
# error This header file can only be included from LibVLC.
#endif

#ifndef _VOUT_INTERNAL_SNAPSHOT_H
#define _VOUT_INTERNAL_SNAPSHOT_H

#include <vlc_picture.h>

typedef struct {
    vlc_mutex_t lock;
    vlc_cond_t  wait;

	bool        is_available;
	int         request_count;
	picture_t   *picture;

} vout_snapshot_t;

/* */
void vout_snapshot_Init(vout_snapshot_t *);
void vout_snapshot_Clean(vout_snapshot_t *);

void vout_snapshot_End(vout_snapshot_t *);

/* */
picture_t *vout_snapshot_Get(vout_snapshot_t *, mtime_t timeout);

/**
 * It tells if they are pending snapshot request
 */
bool vout_snapshot_IsRequested(vout_snapshot_t *);

/**
 * It set the picture used to create the snapshots.
 *
 * The given picture is only copied and not released.
 * If p_fmt is non NULL it will override the format of the p_picture (mainly
 * used because of aspect/crop problems).
 */
void vout_snapshot_Set(vout_snapshot_t *, const video_format_t *, const picture_t *);

/**
 * This function will return the directory used for snapshots
 */
char *vout_snapshot_GetDirectory(void);

typedef struct {
    bool is_sequential;
    int  sequence;
    char *path;
    char *format;
    char *prefix_fmt;
} vout_snapshot_save_cfg_t;

/**
 * This function will write an image to the disk an return the file name created.
 */
int vout_snapshot_SaveImage(char **name, int *sequential,
                            const block_t *image,
                            vlc_object_t *object,
                            const vout_snapshot_save_cfg_t *cfg);

#endif

