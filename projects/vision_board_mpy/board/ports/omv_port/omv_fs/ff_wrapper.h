/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013-2016 Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * File System Helper Functions
 *
 */
#ifndef __FF_WRAPPER_H__
#define __FF_WRAPPER_H__
#include <stdint.h>
#include <stdbool.h>
#include <ff.h>
extern const char *ffs_strerror(FRESULT res);

void file_raise_format(FIL *fp);
void file_raise_corrupted(FIL *fp);
void file_raise_error(FIL *fp, FRESULT res);

// These low-level functions/wrappers don't use file buffering,
// and they don't raise any exceptions, so they're safe to call
// from anywhere.
FRESULT file_ll_open(FIL *fp, const TCHAR *path, BYTE mode);
FRESULT file_ll_close(FIL *fp);
FRESULT file_ll_read(FIL *fp, void *buff, UINT btr, UINT *br);
FRESULT file_ll_write(FIL *fp, const void *buff, UINT btw, UINT *bw);
FRESULT file_ll_opendir(DIR *dp, const TCHAR *path);
FRESULT file_ll_stat(const TCHAR *path, FILINFO *fno);
FRESULT file_ll_mkdir(const TCHAR *path);
FRESULT file_ll_unlink(const TCHAR *path);
FRESULT file_ll_rename(const TCHAR *path_old, const TCHAR *path_new);
FRESULT file_ll_touch(const TCHAR *path);

// File buffer functions.
void file_buffer_init0();
void file_buffer_on(FIL *fp);  // Calls fb_alloc_all()
void file_buffer_off(FIL *fp); // Calls fb_free()

void file_open(FIL *fp, const char *path, bool buffered, uint32_t flags);
void file_close(FIL *fp);
void file_seek(FIL *fp, UINT offset);
void file_truncate(FIL *fp);
void file_sync(FIL *fp);
uint32_t file_tell(FIL *fp);
uint32_t file_size(FIL *fp);
void file_read(FIL *fp, void *data, size_t size);
void file_write(FIL *fp, const void *data, size_t size);
void file_write_byte(FIL *fp, uint8_t value);
void file_write_short(FIL *fp, uint16_t value);
void file_write_long(FIL *fp, uint32_t value);
void file_read_check(FIL *fp, const void *data, size_t size);
// File buffer functions.
void file_buffer_init0();
void file_buffer_on(FIL *fp); // does fb_alloc_all
uint32_t file_tell_w_buf(FIL *fp); // valid between on and off
uint32_t file_size_w_buf(FIL *fp); // valid between on and off
void file_buffer_off(FIL *fp); // does fb_free
void read_byte(FIL *fp, uint8_t *value);
void read_byte_expect(FIL *fp, uint8_t value);
void read_byte_ignore(FIL *fp);
void read_word(FIL *fp, uint16_t *value);
void read_word_expect(FIL *fp, uint16_t value);
void read_word_ignore(FIL *fp);
void read_long(FIL *fp, uint32_t *value);
void read_long_expect(FIL *fp, uint32_t value);
void read_long_ignore(FIL *fp);
void read_data(FIL *fp, void *data, UINT size);
void write_byte(FIL *fp, uint8_t value);
void write_word(FIL *fp, uint16_t value);
void write_long(FIL *fp, uint32_t value);
void write_data(FIL *fp, const void *data, UINT size);
#endif /* __FF_WRAPPER_H__ */
