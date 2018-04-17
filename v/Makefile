export RISCV=/local/scratch/jrrk2/riscv_install
#export RISCV=/local/scratch/jrrk2/lowrisc-chip-rocket-customisation/riscv
export LD_LIBRARY_PATH=/local/scratch-legacy/jrrk2/l3riscv
O =  vm.c hid.c memset.S # mini-printf.c
D = syscalls.c dram.c mini-printf.c random.c bare_metal_include/strlen.c
B = bare.c dram.c mini-printf.c random.c hid.c
B2 = bare2.c dram2.c mini-printf.c random.c hid.c
E = eth.c mini-printf.c random.c hid.c sdhci-minion-hash-md5.c elf.c bare_metal_include/memcmp.c bare_metal_include/memcpy.c bare_metal_include/memset.c bare_metal_include/wordcopy.c
Z = # mydummy.c hid.c mini-printf.c

all: vm addone bare dram check dummy hello goodbye

vm: entry.S $O link.ld dram ./cnvmem
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib entry.S -g -O $O -lc usercode.S -T link.ld -DENTROPY=0xf7930f7 -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	cp -p cnvmem64.hex $@.hex

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
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib dummy.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

hello: hello.S $Z hello.ld ./cnvmem
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib hello.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	cp -p cnvmem64.hex $@.hex

goodbye: goodbye.S $Z goodbye.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib goodbye.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

blank: blank.S $Z blank.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib blank.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

addone: addone.S $Z addone.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc -march=rv64g -nostdlib addone.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

bare: $B
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc -march=rv64g -T $@.ld -nostdlib barecrt.S -static -g -Os $B -o $@
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-linux-gnu-size $@

barec: $B
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc -march=rv64gc -T $@.ld -nostdlib barecrt.S -static -g -Os $B -o $@
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-linux-gnu-size $@

bare2: $(B2)
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc -march=rv64g -T $@.ld -nostdlib barecrt2.S -static -g -Os $(B2) -o $@
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-linux-gnu-size $@

eth: $E
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc -march=rv64g -I.. -Ibare_metal_include -T $@.ld -nostdlib barecrt2.S -static -g -Os $(E) -o $@ -lgcc
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-linux-gnu-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-linux-gnu-size $@

hellotest: hello
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

btest: blank
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

baretest2: bare2
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

ethertest: eth ./recvRawEth bbl
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<
	./recvRawEth -r eth0 boot.bin

etherbad: eth ./recvRawEth bbl
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<
	./recvRawEth -r -m eth0 boot.bin

vmtest: vm
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$< -o40000000

cnvmem: cnvmem.v
	iverilog cnvmem.v -o cnvmem

recvRawEth: recvRawEth.c
	gcc $< -o $@ -g -lssl -lcrypto
	@echo This version of etherboot/ethersd requires super user powers ...
	sudo setcap cap_net_raw+ep $@

bbl: ../../riscv-pk/build/bbl
	env PATH=/local/scratch/jrrk2/riscv_install/bin:$(PATH) make -C ../../riscv-pk/build
	cp -p ../../riscv-pk/build/bbl boot.bin
	$(RISCV)/bin/riscv64-unknown-elf-strip boot.bin
