/**
 * Copyright (C) 2013 by Paul Hooijenga   (paulhooijenga at gmail.com)
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
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/
#ifndef __GPT_H
#define __GPT_H

#include <stdint.h>
#include <stdio.h>

#include "platform.h"

struct GPTGuid {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint64_t Data4;
};

struct GPTHeader {
    char signature[8];
    uint32_t revision;
    uint32_t size;
    uint32_t crc32;
    uint32_t reserved;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_lba;
    uint64_t last_lba;
    struct GPTGuid disk_guid;
    uint64_t partition_lba;
    uint32_t num_partitions;
    uint32_t entry_size;
    uint32_t array_crc32;
};

struct GPTPartition {
    struct GPTGuid type_guid;
    struct GPTGuid guid;
    uint64_t first_lba;
    uint64_t last_lba;
    uint64_t flags;
    uint16_t name[36]; /* UTF-16 LE */
};

#define GPT_HEADER_SIGNATURE           "EFI PART"

static INLINE int valid_gpt_header(const struct GPTHeader* header) {
    return memcmp(header->signature, GPT_HEADER_SIGNATURE, sizeof(header->signature)) == 0;
}

static INLINE int gpt_guid_equal(const struct GPTGuid* a, const struct GPTGuid* b) {
    return memcmp(a, b, sizeof(struct GPTGuid)) == 0;
}

static INLINE int gpt_guid_to_string(char* buf, const struct GPTGuid* guid) {
    unsigned char data4[8];
    memcpy(data4, &guid->Data4, sizeof(uint64_t));

    return sprintf(buf, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                   guid->Data1, guid->Data2, guid->Data3,
                   data4[0], data4[1], data4[2], data4[3],
                   data4[4], data4[5], data4[6], data4[7]);
}

#define GPT_GUID(name, data1, data2, data3, data4a, data4b) \
    const struct GPTGuid gpt_guid_##name = { data1, data2, data3, __builtin_bswap64((uint64_t)data4a << 48 | data4b) }

GPT_GUID(none,          0x00000000, 0x0000, 0x0000, 0x0000, 0x000000000000LL);
GPT_GUID(ms_reserved,   0xE3C9E316, 0x0B5C, 0x4DB8, 0x817D, 0xF92DF00215AELL);
GPT_GUID(ms_basic_data, 0xEBD0A0A2, 0xB9E5, 0x4433, 0x87C0, 0x68B6B72699C7LL);
GPT_GUID(linux_fs_data, 0x0FC63DAF, 0x8483, 0x4772, 0x8E79, 0x3D69D8477DE4LL);
GPT_GUID(linux_home,    0x933AC7E1, 0x2EB4, 0x4F13, 0xB844, 0x0E14E2AEF915LL);

#endif // __GPT_H
