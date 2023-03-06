#----------------------------------------------------------------------------
# cppmoreexample.mak
# copyright (c) innovaphone 2015
#----------------------------------------------------------------------------

OUT = cppmoreexample

include common/build/build.mak

include sdk/platform/sdk-defs.mak

include web1/appwebsocket/appwebsocket.mak
include web1/fonts/fonts.mak
include web1/lib1/lib1.mak
include web1/ui1.lib/ui1.lib.mak

-include cppmoreexample/submodules.mak
include cppmoreexample/cppmoreexample.mak

APP_OBJS += $(OUTDIR)/obj/cppmoreexample-main.o
$(OUTDIR)/obj/cppmoreexample-main.o: cppmoreexample-main.cpp force

include sdk/platform/sdk.mak


