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

    // process the directories
    s64 current_dir = this->directory_count - 1;
    for (; current_dir >= 0; current_dir--) {
        current_location -= RKV1_DIRECTORY_NAME_LENGTH;
        this->directory_entries[current_dir].index = current_dir;
        this->directory_entries[current_dir].name_offset = current_location;
    }
    printf("directories processed\n");

    // process the files
    for (s64 f = this->file_count - 1; f >= 0; f--) {
        current_location -= RKV1_FILE_ENTRY_SIZE;
        this->file_entries[f].name_offset = current_location;
        this->file_entries[f].directory_index = read_32LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH]);
        this->file_entries[f].file_size = read_64LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH + 0x4]);
        this->file_entries[f].data_pointer = read_32LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH + 0xC]);
        this->file_entries[f].checksum = read_32LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH + 0x10]);
        this->file_entries[f].timestamp = read_32LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH + 0x14]);
        this->file_entries[f].uid = read_32LE(&this->data[current_location + RKV1_FILE_NAME_LENGTH + 0x18]);
        printf("File ID: 0x%.4X - Checksum: 0x%.4X - Name: %s\n", this->file_entries[f].uid, this->file_entries[f].checksum,
                &this->data[this->file_entries[f].name_offset]);
    }
    printf("files processed\n");

    return true;
}
