savedcmd_mutex_benchmark.ko := ld.lld -r -m elf_x86_64 -z noexecstack --build-id=sha1  -T /home/rustxave/Scrivania/6.13/linux/scripts/module.lds -o mutex_benchmark.ko mutex_benchmark.o mutex_benchmark.mod.o .module-common.o