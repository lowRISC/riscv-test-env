#export RISCV=/local/scratch/jrrk2/riscv_install
export RISCV=/local/scratch-legacy/jrrk2/riscv_install
#export RISCV=/local/scratch/jrrk2/lowrisc-chip-rocket-customisation/riscv
export LD_LIBRARY_PATH=/local/scratch-legacy/jrrk2/l3riscv
O =  vm.c hid.c memset.S # mini-printf.c
D = syscalls.c dram.c mini-printf.c random.c bare_metal_include/strlen.c
F = syscalls.c fork.c mini-printf.c random.c bare_metal_include/strlen.c bare_metal_include/string/strchr.c bare_metal_include/string/strcmp.c
B = bare.c dram.c mini-printf.c random.c hid.c
B2 = bare2.c dram2.c mini-printf.c random.c hid.c common.c memset.S
E = eth.c dhcp-client.c mini-printf.c random.c hid.c sdhci-minion-hash-md5.c elf.c common.c bare_metal_include/memcmp.c bare_metal_include/memcpy.c bare_metal_include/memset.c bare_metal_include/wordcopy.c bare_metal_include/strlen.c bare_metal_include/strcpy.c
I = int.c mini-printf.c random.c hid.c sdhci-minion-hash-md5.c elf.c bare_metal_include/memcmp.c bare_metal_include/memcpy.c bare_metal_include/memset.c bare_metal_include/wordcopy.c bare_metal_include/strlen.c bare_metal_include/strcpy.c
U = bare.c uart1.c mini-printf.c hid.c
Z = # mydummy.c hid.c mini-printf.c
CFLAGS = -march=rv64g -nostdlib -I.. -save-temps
all: vm addone bare dram check dummy hello goodbye

vm: entry.S $O link.ld dram ./cnvmem
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) entry.S -g -O $O -lc usercode.S -T link.ld -DENTROPY=0xf7930f7 -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	cp -p cnvmem64.hex $@.hex

dram: $D
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc $(CFLAGS) -T dram.ld -static -g -Os $D -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@

check: $D
	cc -g -Os $D -nostdlib -lc -o $@
	size $@
	./$@

dummy: dummy.S $Z dummy.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) dummy.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

hello: hello.S $Z hello.ld ./cnvmem
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) hello.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	cp -p cnvmem64.hex $@.hex

goodbye: goodbye.S $Z goodbye.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) goodbye.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

blank: blank.S $Z blank.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) blank.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

addone: addone.S $Z addone.ld
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) addone.S -g -O $Z -lc -T $@.ld -mcmodel=medany -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis

bare: $B
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc $(CFLAGS) -T $@.ld barecrt.S -static -g -Os $B -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@

barec: $B
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS)c -T $@.ld barecrt.S -static -g -Os $B -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@

bare2: $(B2)
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) -T $@.ld barecrt2.S -static -g -Os $(B2) -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	mv cnvmem64.hex $@.hex
	rm cnvmem.coe cnvmem.hex

uart: $U
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) -T $@.ld barecrt2.S -static -g -Os $U -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem 
	./cnvmem
	mv cnvmem64.hex $@.hex
	rm cnvmem.coe cnvmem.hex

eth: $E lowrisc.dtb
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) -I.. -Ibare_metal_include -T $@.ld ethcrt.S -static -g -Os $(E) -o $@ -lgcc
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem
	$(RISCV)/bin/riscv64-unknown-elf-size $@
	./cnvmem
	mv cnvmem64.hex $@.hex
	rm cnvmem.coe cnvmem.hex

int: $I
	$(RISCV)/bin/riscv64-unknown-elf-gcc $(CFLAGS) -I.. -Ibare_metal_include -T $@.ld intcrt.S -static -g -Os $(I) -o $@ -lgcc
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog $@.bin cnvmem.mem
	$(RISCV)/bin/riscv64-unknown-elf-size $@
	./cnvmem
	mv cnvmem64.hex $@.hex
	rm cnvmem.coe cnvmem.hex

intdummy: int
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

hellotest: hello
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

btest: blank
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

baretest: bare
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

baretest2: bare2
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

uarttest: uart
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<

ethertest: eth ./recvRawEth boot.bin
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<
	time ./recvRawEth -s 192.168.0.51 boot.bin

etherdummy: eth ./recvRawEth
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<
	./recvRawEth -s 192.168.0.51 dummy.bin

uboot: eth ./recvRawEth uboot.bin
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$<
	./recvRawEth eth0 uboot.bin

vmtest: vm
	../../jtag_digilent_ariane/jtag_digilent_ariane -r$< -o40000000

cnvmem: cnvmem.v
	iverilog cnvmem.v -o cnvmem

recvRawEth: recvRawEth.c
	gcc $< -o $@ -g -lssl -lcrypto
#	@echo This version of etherboot/ethersd requires super user powers ...
#	sudo setcap cap_net_raw+ep $@

boot.bin: ../../riscv-pk/build/bbl ./cnvmem
	env PATH=/local/scratch-legacy/jrrk2/riscv_install/bin:$(PATH) make -C ../../riscv-pk/build
	cp -p ../../riscv-pk/build/bbl boot.bin
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -S -l boot.bin > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-strip boot.bin

uboot.bin: ../../u-boot-riscv/u-boot ./cnvmem
	cp -p ../../u-boot-riscv/u-boot uboot.bin
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -S -l uboot.bin > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-strip uboot.bin

.PHONY: ../../u-boot-riscv/u-boot
../../u-boot-riscv/u-boot:
	env PATH=/local/scratch-legacy/jrrk2/riscv_install/bin:$(PATH) make -C ../../u-boot-riscv -j 4

dummy.bin: ../../riscv-pk/dummy/bbl ./cnvmem
	env PATH=/local/scratch-legacy/jrrk2/riscv_install/bin:$(PATH) make -C ../../riscv-pk/dummy
	cp -p ../../riscv-pk/dummy/bbl dummy.bin
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -S -l dummy.bin > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-strip dummy.bin

bbl.hex: boot.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary boot.bin bbl.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog bbl.bin cnvmem.mem
	./cnvmem
	mv cnvmem64.hex $@

spike.hex: spike.S
	$(RISCV)/bin/riscv64-unknown-elf-gcc -mabi=lp64 -march=rv64imafd -nostdlib $<
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -l -S a.out >spike.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary a.out a.bin
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I binary -O verilog a.bin cnvmem.mem
	./cnvmem
	mv cnvmem64.hex $@

program: /local/scratch/jrrk2/ariane/my_svf_target.svf
	../../jtag_digilent_ariane/jtag_digilent_ariane $<

grab:
	../../jtag_digilent_ariane/jtag_digilent_ariane -g

dummylnx: fork dram
	rm -fr ramfs
	mkdir ramfs
	mkdir -p ramfs/bin ramfs/etc ramfs/dev ramfs/lib ramfs/proc ramfs/tmp
	cp dram ramfs/bin
	cp $< ramfs/init
	cd ramfs; echo "\
        mknod dev/null c 1 3 && \
        mknod dev/tty c 5 0 && \
        mknod dev/zero c 1 5 && \
        mknod dev/console c 5 1 && \
        mknod dev/random c 1 8 && \
        mknod dev/urandom c 1 9 && \
        mknod dev/mmcblk0 b 179 0 && \
        mknod dev/mmcblk0p1 b 179 1 && \
        mknod dev/mmcblk0p2 b 179 2 && \
        find . | cpio -H newc -o > ../../../riscv-linux/initramfs.cpio" | fakeroot

fork: $F
	$(RISCV)/bin/riscv64-unknown-linux-gnu-gcc $(CFLAGS) -T dram.ld -static -g -Os $F -o $@
	$(RISCV)/bin/riscv64-unknown-elf-objdump -d -D -z -S -l $@ > $@.dis
	$(RISCV)/bin/riscv64-unknown-elf-objcopy -I elf64-little -O binary $@ $@.bin
	$(RISCV)/bin/riscv64-unknown-elf-size $@
