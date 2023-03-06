include cppmoreexample/apps/apps.mak

APP_OBJS += $(OUTDIR)/obj/cppmoreexample.o
$(OUTDIR)/obj/cppmoreexample.o: cppmoreexample/cppmoreexample.cpp $(OUTDIR)/cppmoreexample/cppmoreexample.png

APP_OBJS += $(OUTDIR)/obj/cppmoreexample_tasks.o
$(OUTDIR)/obj/cppmoreexample_tasks.o: cppmoreexample/cppmoreexample_tasks.cpp

APP_OBJS += $(OUTDIR)/obj/alphabet_api.o
$(OUTDIR)/obj/alphabet_api.o: cppmoreexample/alphabet_api.cpp

APP_OBJS += $(OUTDIR)/obj/alphabet_db_task.o
$(OUTDIR)/obj/alphabet_db_task.o: cppmoreexample/alphabet_db_task.cpp

$(OUTDIR)/cppmoreexample/cppmoreexample.png: cppmoreexample/cppmoreexample.png
	copy cppmoreexample\cppmoreexample.png $(OUTDIR)\cppmoreexample\cppmoreexample.png
