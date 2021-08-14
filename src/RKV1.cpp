#include "RKV1.h"
#include "rw_utilities/mem_read_utilities.h"
#include <stdio.h>
#include <cstring>

bool RKV1File::load(const u8* buf_to_copy, const u64 buf_size) {
    // copy data into our buffer
    this->data = new u8[buf_size];
    this->data_size = buf_size;
    memcpy(this->data, buf_to_copy, buf_size);
    printf("past copy\n");

    // we don't really have a way to verify the header, so we're just gonna start loading and hope for the best lol
    // set our current location to the variable which holds the directory count
    u64 current_location = this->data_size - 0x4;
    this->directory_count = read_32LE(&this->data[current_location]);
    current_location -= 0x4;
    this->file_count = read_32LE(&this->data[current_location]);

    if (this->directory_count == 0 || this->file_count == 0) {
        printf("Invalid directory or file counts: %d directories detected, %d files detected\n", this->directory_count, this->file_count);
        return false;
    }

    // allocate the directories and files
    this->directory_entries = new RKV1DirectoryEntry[this->directory_count];
    this->file_entries = new RKV1FileEntry[this->file_count];
    printf("metadata processed, entries allocated\n");

    s32 current_dir = this->directory_count - 1;
    for (; current_dir >= 0; current_dir--) {
        current_location -= RKV1_DIRECTORY_NAME_LENGTH;
        this->directory_entries[current_dir].index = current_dir;
        this->directory_entries[current_dir].name_offset = current_location;
        printf("Directory %d: %s\n", this->directory_entries[current_dir].index, &this->data[this->directory_entries[current_dir].name_offset]);
    }

    return true;
}
