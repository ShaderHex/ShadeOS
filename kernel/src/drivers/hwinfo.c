#include "hwinfo.h"

volatile struct limine_memmap_request memmap_request
    __attribute__((used, section(".limine_requests"))) = {
        .id = LIMINE_MEMMAP_REQUEST_ID,
        .revision = 0
};


void u64_to_hex(uint64_t value, char *buffer, int digits) {
    const char *hex = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 0; i < digits; i++) {
        int shift = (digits - 1 - i) * 4;
        buffer[2 + i] = hex[(value >> shift) & 0xF];
    }
    buffer[2 + digits] = '\0';
}

void u64_to_dec(uint64_t value, char *buffer) {
    char temp[21];
    int i = 0;
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}

void print_memmap() {
    struct limine_memmap_response *resp = memmap_request.response;
    if (!resp) {
        print_string("Memmap not available!\n", 0xFFFFFFFF);
        return;
    }

    char buf[64];
    for (uint64_t i = 0; i < resp->entry_count; i++) {
        struct limine_memmap_entry *e = resp->entries[i];

        u64_to_dec(i, buf);
        print_string("Entry ", 0xFFFFFFFF);
        print_string(buf, 0xFFFFFFFF);
        print_string(": Base=", 0xFFFFFFFF);

        u64_to_hex(e->base, buf, 16);
        print_string(buf, 0xFFFFFFFF);
        print_string(" Size=", 0xFFFFFFFF);

        u64_to_hex(e->length, buf, 16);
        print_string(buf, 0xFFFFFFFF);
        print_string(" Type=", 0xFFFFFFFF);

        u64_to_dec(e->type, buf);
        print_string(buf, 0xFFFFFFFF);
        print_string("\n", 0xFFFFFFFF);
    }
}


void print_memory_info_detailed() {
    if (memmap_request.response == NULL) {
        print_string("No memory map available!\n", 0xFFFFFFFF);
        return;
    }

    uint64_t total_usable = 0;

    for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *e = memmap_request.response->entries[i];

        char numbuf[32];
        int_to_string(i, numbuf);
        print_string("Entry #", 0xFFFFFFFF);
        print_string(numbuf, 0xFFFFFFFF);
        print_string(": ", 0xFFFFFFFF);

        switch (e->type) {
            case 0: {
                print_string("Firmware reserved / Not usable\n", 0xFFFF0000);
                break;
            }
            case 1: {
                print_string("Usable memory: ", 0xFF00FF00);

                total_usable += e->length;

                char sizebuf[64];
                int_to_string(total_usable, sizebuf);
                print_string(sizebuf, 0xFFFFFFFF);
                print_string("\n", 0xFFFFFFFF);
                break;
            }
            case 2: {
                print_string("Reserved / Not usable\n", 0xFFFF0000);
                break;
            }
            case 3: {
                print_string("ACPI Reclaimable\n", 0xFFFFFF00);
                break;
            }
            case 4: {
                print_string("ACPI NVS / Not usable\n", 0xFFFF8800);
                break;
            }
            case 5: {
                print_string("Bad memory / Not usable\n", 0xFFFF0000);
                break;
            }
            default: {
                print_string("Unknown type\n", 0xFFFFFFFF);
                break;
            }
        }
    }
}


void print_memory_info() {
    uint64_t total_usable = 0;

    for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *e = memmap_request.response->entries[i];
        if (e->type == LIMINE_MEMMAP_USABLE) {
            total_usable += e->length;
        }
    }

    char buf[64];

    u64_to_hex(total_usable, buf, 18);
    print_string("Total usable RAM (bytes): ", 0xFFFFFFFF);
    print_string(buf, 0xFFFFFFFF);
    print_string("\n", 0xFFFFFFFF);

    uint64_t mb = total_usable / (1024 * 1024);
    int_to_string(mb, buf);
    print_string("Total usable RAM (MB): ", 0xFFFFFFFF);
    print_string(buf, 0xFFFFFFFF);
    print_string("\n", 0xFFFFFFFF);
}
