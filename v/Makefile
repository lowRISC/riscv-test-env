O =  vm.c hid.c # mini-printf.c
D = syscalls.c dram.c mini-printf.c random.c

vm: entry.S $O link.ld dram
	riscv64-unknown-elf-gcc -nostdlib entry.S -g -O $O -lc usercode.S -T link.ld -DENTROPY=0xf7930f7 -mcmodel=medany -o $@
	riscv64-unknown-elf-objdump -d -D -z -S -l vm > $@.dis
	riscv64-unknown-elf-objcopy -I elf64-little -O binary vm $@.bin
	riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	iverilog cnvmem.v -o cnvmem
	./cnvmem

dram: $D
	riscv64-unknown-linux-gnu-gcc -T dram.ld -nostdlib -static -g -Os $D -o $@
	riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	riscv64-unknown-linux-gnu-size $@

check: $D
	cc -g -Os $D -nostdlib -lc -o $@
	size $@
	./$@
