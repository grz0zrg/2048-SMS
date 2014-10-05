CC = wla-z80
CFLAGS = -vo
LD = wlalink
LDFLAGS = -vdrs

ASMFILE = main.asm
OFILE = main.o
OUTPUTFILE = out.sms
LINKFILE = linkfile

all: clean
	@echo " ==== Compiling: $(CC) $(CFLAGS) $(ASMFILE) $(OFILE) \n"
	@$(CC) $(CFLAGS) $(ASMFILE) $(OFILE)
	@echo "\n ==== Linking: $(LD) $(LDFLAGS) $(LINKFILE) $(OUTPUTFILE) \n"
	@$(LD) $(LDFLAGS) $(LINKFILE) $(OUTPUTFILE)

clean:
	@echo " ==== Cleaning: rm -f $(OFILE) core *~ $(OUTPUTFILE) linked.sym"
	@rm -f $(OFILE) core *~ $(OUTPUTFILE) out.sym
