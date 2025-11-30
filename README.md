# ShadeOS

not much to see as even the readme is under construction

## Screenshot
------------------------------------------
![Kernel Screenshot](screenshot.png)

## Feature Plans
- [x] Bootloader Limine
- [x] GDT, IDT 
- [x] TSC  
- [x] Basic drivers  
- [x] Panic screen  
- [ ] Scheduler  
- [ ] Syscall  
- [ ] User space / kernel space  
- [ ] HESP (the timer)
- [ ] ACPI
- [ ] GUI

*continues…*

## Build
-----------------------------
To build the kernel:  
```bash
make
````

To build and run in QEMU:

```bash
make run
```

To build and debug:

```bash
make debug
```

To clean build files:

```bash
make clean
```

## License

Licensed under **GPL 3.0**
