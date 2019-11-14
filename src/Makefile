include make.in
TOOLS=/projects/robot/tools
PREFIX=.
include settings.in
OBJ-crt0=basic/crt0.o
OBJ-BASIC=basic/sys_var.o basic/ttyio.o basic/qsm.o basic/io_sci.o basic/qsmWrapper.o basic/io_duart.o basic/duartWrapper.o basic/e_handle.o basic/ptimer.o basic/context.o basic/kerror.o basic/schedule.o basic/trap.o basic/list.o basic/ivtab.o basic/sleep.o basic/softreset.o
OBJ-TEST=basic/sys_var.o basic/io_duart.o basic/e_handle.o basic/kerror.o 
OBJ-INIT=init/init.o init/readcmd.o init/tpuisrw.o init/tputest.o init/schedtest.o init/i2ctest.o init/radiotest.o init/listtest.o init/hwinfot.o init/fbintest.o init/resettest.o
OBJ-FASTBIN=drivers/fastbin/fastbin.o
OBJ-TPU=drivers/tpu/tpud.o drivers/tpu/sai_tpu.o
OBJ-I2C=drivers/i2c/i2cd.o drivers/i2c/i2caint.o drivers/i2c/i2cbint.o drivers/i2c/sai_i2c.o drivers/i2c/charger.o drivers/i2c/fuelgauge.o drivers/i2c/analog.o drivers/i2c/digital.o
OBJ-ICONTROL=drivers/icontrol/icontrolWrapper.o drivers/icontrol/icontrol.o drivers/icontrol/sai_motor.o drivers/icontrol/sincos.o
OBJ-DRIVERS=$(OBJ-TPU) $(OBJ-I2C) $(OBJ-FASTBIN) drivers/sai.o drivers/sai_cube.o
OBJ-CUBE=cube/kio.o cube/close.o cube/fstat.o cube/kill.o cube/print.o cube/stat.o cube/putnum.o cube/lseek.o cube/read.o cube/unlink.o cube/isatty.o cube/open.o cube/sbrk.o cube/write.o cube/getpid.o
OBJ-NET=net/network.o net/rsm.o
LINKSCRIPT=cubeos.ld_sun
TEST_LINKSCRIPT=test.ld_sun
OBJ-ALL=$(OBJ-BASIC) $(OBJ-NET) $(OBJ-DRIVERS) $(OBJ-CUBE)

all: init.abs init.tw

 
lib: libcubeos.a

indent: 
	find . -name \*.c -ls -exec indent {} \;

removebaks: 
	find . -name \*.c~ -ls -exec /bin/rm {} \;

tar: indent removebaks clean all clean
	-rm ../src-old.tgz
	-mv ../src.tgz ../src-old.tgz
	( cd ..; gtar -zcvf src.tgz src )
		

basic/crt0.o: basic/ivtable.S

test/crt0.o: test/ivtable.S

libcubeos.a: $(OBJ-crt0) $(OBJ-ALL) $(LINKSCRIPT) Makefile
	-rm -f libcubeos.a
	$(AR) rcsv libcubeos.a $(OBJ-ALL)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(SFLAGS) -c $< -o $@

.coff.d:
	$(OBJDUMP) -d $< >$@

init.coff: $(OBJ-crt0) $(OBJ-ALL) $(OBJ-INIT) $(LINKSCRIPT)
	$(LD) $(LD_OPT) -Map  $*.map -o $@ $(OBJ-ALL) $(OBJ-INIT) -T$(LINKSCRIPT)

initl.coff: libcubeos.a $(OBJ-INIT) $(LINKSCRIPT)
	$(LD) $(LD_OPT) -Map  $*.map  -o $@ $(OBJ-INIT)  -Tcubeos.ld_lib


.coff.abs:
	$(OBJCOPY) -O srec $< $@

.abs.tw:
	$(TOOLS)/s2tw $< $@


test: test.coff

test.coff: $(OBJ-TEST) $(TEST_LINKSCRIPT) test/ivtable.S test/crt0.o test/test.o test/kio.o test/doprnt.o test/bcopy.o
	$(LD) $(LD_OPT) -Map  $*.map -o $@ $(OBJ-TEST) test/test.o test/kio.o test/doprnt.o test/bcopy.o -T$(TEST_LINKSCRIPT)


clean:
	-rm net/*.o init/*.o cube/*.o basic/*.o drivers/*.o drivers/tpu/*.o drivers/i2c/*.o drivers/icontrol/*.o drivers/fastbin/*.o *.coff *.bin *.d *.abs *.lib *.d *.map

