


CC := g++
FLAG = -g

DPF:= .dpf

####################### include head file dependence #####################

$(shell rm -f $(DPF))
$(foreach src,$(SRC_LIST),\
	$(shell	$(CC) -MM $(FLAG) $(INCLUDE) $(src) | \
	sed 's/^\(.*\)\.o/obj\/\1\.o/'>> $(DPF);\
	))
	

sinclude $(DPF)

