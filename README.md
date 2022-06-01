# bootloader
This project is a simple bootloader program.
## How to compile code
After set up toolchain correctly, you can compile the bootloader project using the cmake:
```shell
cd <root-folder-of-this-repo>
mkdir build
cmake ..
make -j
```
## file structure

```c
bootloader
    ├─arch  .......... about cpu  
    │  ├─chip1 ....... about specific cpu chip  
    │  │  ├─driver ... driver code like HAL libiary  
    │  │  │  ├─inc  
    │  │  │  └─src  
    │  │  └─include .. specific cpu chip register defines  
    │  ├─include ..... this cpu architecture general defines  
    │  └─lib ......... this cpu architecture general code  
    └─lib ............ some lib maybe useful, but now empty  
        ├─libc  
        │  ├─crypto  
        │  ├─ctype  
        │  ├─stdio  
        │  └─stdlib  
        └─libm  
    kservice.c ...... some functions about printk or other general for bootloader  
    main.c .......... bootloader main function  
    download.c ...... code of download kernel and img from other device  
```
    
