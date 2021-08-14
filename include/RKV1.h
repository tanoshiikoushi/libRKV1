#ifndef RKV1_H_INCLUDED

#include <types/typedefs.h>

#define RKV1_H_INCLUDED

const u8 RKV1_FILE_NAME_LENGTH = 0x20;
const u16 RKV1_DIRECTORY_NAME_LENGTH = 0x100;

class RKV1DirectoryEntry {
    private:
        u64 name_offset;
        u32 index;
};

class RKV1FileEntry {
    private:
        u64 name_offset;
        u32 directory_index;
        u64 file_size;
        u32 data_pointer;
        u32 checksum;
        u32 timestamp; //this is also used to match duplicates, so we need to preserve this
        u32 uid;
};

class RKV1File {
    private:
        u32 file_count;
        RKV1FileEntry* file_entries;
        u32 directory_count;
        RKV1DirectoryEntry* directory_entries;
        u64 data_size;
        u8* data;

    public:
        bool load(const u8* rkv_buf, const u64 buf_size);
        bool extract(const u8* output_path);

    ~RKV1File() {
        if (data) {
            delete data;
        }

        if (file_entries) {
            delete[] file_entries;
        }

        if (directory_entries) {
            delete directory_entries;
        }
    }
};

#endif
