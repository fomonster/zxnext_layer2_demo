################################################################################
# Stefan Bylund 2017
#
# Makefile for compiling layer 2 demo programs for Sinclair ZX Spectrum Next.
# Supports both the SCCZ80 and SDCC compilers.
################################################################################

MKDIR := mkdir -p

RM := rm -rf

CP := cp

ZIP := zip -r -q

ZXNEXT_LAYER2 := ../zxnext_layer2

ZXNEXT_LAYER2_INCLUDE := $(ZXNEXT_LAYER2)/include

ZXNEXT_LAYER2_LIB_SCCZ80 := $(ZXNEXT_LAYER2)/lib/sccz80

ZXNEXT_LAYER2_LIB_SDCC_IX := $(ZXNEXT_LAYER2)/lib/sdcc_ix

ZXNEXT_LAYER2_LIB_SDCC_IY := $(ZXNEXT_LAYER2)/lib/sdcc_iy

BINDIR_SCCZ80 := bin/sccz80

BINDIR_SDCC_IX := bin/sdcc_ix

BINDIR_SDCC_IY := bin/sdcc_iy

ifeq ($(BUILD_SNA),true)
SNA := -subtype=sna
endif

DEBUGFLAGS := --list --c-code-in-asm

all: all_sccz80 all_sdcc_ix all_sdcc_iy

all_sccz80:
	$(MKDIR) $(BINDIR_SCCZ80)
	zcc +zxn $(SNA) -vn -O3 -startup=30 -clib=new -m $(DEBUG) -L$(ZXNEXT_LAYER2_LIB_SCCZ80) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_draw_demo.c -o $(BINDIR_SCCZ80)/zxnext_draw_demo -create-app
	zcc +zxn $(SNA) -vn -O3 -startup=30 -clib=new -m $(DEBUG) -L$(ZXNEXT_LAYER2_LIB_SCCZ80) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_perf_demo.c -o $(BINDIR_SCCZ80)/zxnext_perf_demo -create-app
	zcc +zxn $(SNA) -vn -O3 -startup=30 -clib=new -m $(DEBUG) -L$(ZXNEXT_LAYER2_LIB_SCCZ80) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_scroll_demo.c -o $(BINDIR_SCCZ80)/zxnext_scroll_demo -create-app
	zcc +zxn $(SNA) -vn -O3 -startup=30 -clib=new -m $(DEBUG) -L$(ZXNEXT_LAYER2_LIB_SCCZ80) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_misc_demo.c -o $(BINDIR_SCCZ80)/zxnext_misc_demo -create-app

all_sdcc_ix:
	$(MKDIR) $(BINDIR_SDCC_IX)
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_ix -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IX) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_draw_demo.c -o $(BINDIR_SDCC_IX)/zxnext_draw_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_ix -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IX) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_perf_demo.c -o $(BINDIR_SDCC_IX)/zxnext_perf_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_ix -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IX) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_scroll_demo.c -o $(BINDIR_SDCC_IX)/zxnext_scroll_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_ix -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IX) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_misc_demo.c -o $(BINDIR_SDCC_IX)/zxnext_misc_demo -create-app

all_sdcc_iy:
	$(MKDIR) $(BINDIR_SDCC_IY)
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_iy -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IY) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_draw_demo.c -o $(BINDIR_SDCC_IY)/zxnext_draw_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_iy -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IY) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_perf_demo.c -o $(BINDIR_SDCC_IY)/zxnext_perf_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_iy -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IY) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_scroll_demo.c -o $(BINDIR_SDCC_IY)/zxnext_scroll_demo -create-app
	zcc +zxn $(SNA) -vn -SO3 -startup=30 -clib=sdcc_iy -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IY) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) src/zxnext_misc_demo.c -o $(BINDIR_SDCC_IY)/zxnext_misc_demo -create-app

debug_sccz80: DEBUG = $(DEBUGFLAGS)

debug_sdcc_ix: DEBUG = $(DEBUGFLAGS)

debug_sdcc_iy: DEBUG = $(DEBUGFLAGS)

debug_sccz80: all_sccz80

debug_sdcc_ix: all_sdcc_ix

debug_sdcc_iy: all_sdcc_iy

distro:
	$(MAKE) clean all
	$(MAKE) all BUILD_SNA=true
	$(RM) tmp
	$(MKDIR) tmp/zxnext_layer2_demo
	$(CP) bin/sdcc_iy/*.tap tmp/zxnext_layer2_demo
	$(CP) bin/sdcc_iy/*.sna tmp/zxnext_layer2_demo
	$(CP) resources/*.nxi tmp/zxnext_layer2_demo
	$(CP) src/*.c tmp/zxnext_layer2_demo
	$(CP) readme.txt tmp/zxnext_layer2_demo
	$(RM) build/zxnext_layer2_demo.zip
	cd tmp; $(ZIP) ../build/zxnext_layer2_demo.zip zxnext_layer2_demo
	$(RM) tmp

clean:
	$(RM) bin tmp zcc_opt.def zcc_proj.lst src/*.lis
