export RISCV=/local/scratch/jrrk2/lowrisc-chip-rocket-customisation/riscv
O =  vm.c hid.c memset.S # mini-printf.c
D = syscalls.c dram.c mini-printf.c random.c
Z = # mydummy.c hid.c mini-printf.c

vm: entry.S $O link.ld dram
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib entry.S -g -O $O -lc usercode.S -T link.ld -DENTROPY=0xf7930f7 -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	iverilog cnvmem.v -o cnvmem
	./cnvmem

dram: $D
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc -march=rv64g -T dram.ld -nostdlib -static -g -Os $D -o $@
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-linux-gnu-size $@

check: $D
	cc -g -Os $D -nostdlib -lc -o $@
	size $@
	./$@

dummy: dummy.S $Z dummy.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib dummy.S -g -O $Z -lc -T dummy.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
