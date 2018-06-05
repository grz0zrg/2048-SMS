CC = wla-z80
CFLAGS = -v
LD = wlalink
LDFLAGS = -v -d -r -s

ASMFILE = main.asm
OFILE = main.o
OUTPUTFILE = out.sms
LINKFILE = linkfile

all: clean
	@echo " ==== Compiling: $(CC) $(CFLAGS) -o $(OFILE) $(ASMFILE) \n"
	@$(CC) $(CFLAGS) -o $(OFILE) $(ASMFILE)
	@echo "\n ==== Linking: $(LD) $(LDFLAGS) $(LINKFILE) $(OUTPUTFILE) \n"
	@$(LD) $(LDFLAGS) $(LINKFILE) $(OUTPUTFILE)

clean:
	@echo " ==== Cleaning: rm -f $(OFILE) core *~ $(OUTPUTFILE) linked.sym"
	@rm -f $(OFILE) core *~ $(OUTPUTFILE) out.sym
