O =  vm.c hid.c # mini-printf.c

vm: entry.S $O link.ld
	riscv64-unknown-elf-gcc -nostdlib entry.S -g -O $O -lc usercode.S -T link.ld -DENTROPY=0xf7930f7 -mcmodel=medany -o $@
	riscv64-unknown-elf-objdump -d -D -z vm > vm.dis
	riscv64-unknown-elf-objcopy -I elf64-little -O binary vm vm.bin
	riscv64-unknown-elf-objcopy -I binary -O verilog vm.bin cnvmem.mem 
	iverilog cnvmem.v -o cnvmem
	./cnvmem
