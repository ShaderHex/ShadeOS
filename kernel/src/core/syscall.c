#include "syscall.h"

uint32_t syscall(SyscallID id, uint32_t arg0, uint32_t arg1, uint32_t arg2) {
    switch (id) {
        case SYSCALL_PRINT_STRING:
            print_string((char*)arg0, (unsigned char)arg1);
            return 0;
        case SYSCALL_GET_KERNEL_ADDR:
            
    }
}