default : calc.bin ctests.bin gameoflife.bin hello.bin pi.bin fib.bin

.FORCE : 

compare : kadai.old.bin kadai.bin
	bash -c "time ./kadai.old.bin > /dev/null"
	bash -c "time ./kadai.bin > /dev/null"

compare2 : kadai2.old.bin kadai2.bin
	bash -c "time ./kadai2.old.bin > /dev/null"
	bash -c "time ./kadai2.bin > /dev/null"

run_ctests : ctests.bin ../compilium
	make run_ctests_hostcc
	./ctests.bin

run_ctests_hostcc : ctests.host.bin
	./ctests.host.bin

validate : run_ctests_hostcc

../compilium : .FORCE
	make -C .. compilium

../compilium_dbg : .FORCE
	make -C .. compilium_dbg

constsum.c : gen_constsum.js
	node gen_constsum.js > $@

%.host.bin : %.c Makefile
	$(CC) -o $*.host.bin $*.c

%.host_o3.bin : %.c Makefile
	$(CC) -O3 -o $*.host_o3.bin $*.c

LLDB_ARGS = -o 'settings set interpreter.prompt-on-quit false' \
			-o 'b __assert' \
			-o 'b ErrorWithToken' \
			-o 'process launch'

debug_% : ../compilium_dbg %.c
	lldb \
		-o 'settings set target.input-path $*.c' $(LLDB_ARGS) \
		-- ../compilium_dbg --target-os `uname` -I ../include/

%.host.S : %.c Makefile ../compilium .FORCE
	$(CC) -S -o $@ $*.c
	
%.o0.S : %.c Makefile ../compilium .FORCE
	../compilium -O0 --target-os `uname` -I ../include/ < $*.c > $*.o0.S

%.S : %.c Makefile ../compilium .FORCE
	../compilium --target-os `uname` -I ../include/ < $*.c > $*.S

%.o0.bin : %.o0.S Makefile
	$(CC) -o $@ $*.o0.S

.PRECIOUS : kadai.old.S kadai.S kadai2.old.S kadai2.S

%.old.S : %.c Makefile .FORCE
	../compilium.old --target-os `uname` -I ../include/ < $*.c > $*.old.S

%.old.bin : %.old.S Makefile
	$(CC) -o $@ $*.old.S

format:
	clang-format -i *.c

%.bin : %.S Makefile
	$(CC) -o $@ $*.S

clean:
	-rm *.bin
	-rm *.S
