

TARGET:= tsf

SRC_LIST:= main.c menu.c

OBJ_LIST:=$(patsubst %.c, obj/%.o,$(SRC_LIST))

MDL_LIST :=  dvb format content portable utility expat

CLEAN_LIST := $(addsuffix _c,$(MDL_LIST))

TEST_LIST :=$(addsuffix _t,$(MDL_LIST))

BUILD_LIST:=$(addsuffix _b,$(MDL_LIST))

ARC_LIST := $(patsubst %, lib/%.a,$(MDL_LIST))

INCLUDE := $(addprefix -I../, $(MDL_LIST))

$(TARGET):  $(OBJ_LIST) $(ARC_LIST)
	@echo --------------------- make target $(@)-------------------------------
	$(CC) $(FLAG) -o $@ $^ 
	@echo --------------------- build finished-------------------------------
	@echo 

$(ARC_LIST):$(BUILD_LIST)

$(BUILD_LIST):%:
	@echo -------------------- build module $(subst _b,,$@)------------------
	@echo
	@cd ../$(subst _b,,$@)/ && make 
	@echo

$(CLEAN_LIST):%:
	@echo ------------------- clean module $(subst _c,,$@)-------------------
	cd ../$(subst _c,,$@)/ && make clean

$(TEST_LIST):%:
	@echo ------------------- test module $(subst _t,,$@)--------------------
	cd ../$(subst _t,,$@)/ && make test

$(OBJ_LIST):obj/%.o: %.c
	@echo ------------------- make obj $@ --------------------------------
	$(CC) $(FLAG) $(INCLUDE) -c -o $@ $<

test:$(TEST_LIST)
	@echo TARGET=$(TARGET)
	@echo MDL_LIST=$(MDL_LIST)
	@echo ARC_LIST=$(ARC_LIST)
	@echo TEST_LIST=$(TEST_LIST)
	@echo CLEAN_LIST=$(CLEAN_LIST)
	@echo INCLUDE=$(INCLUDE)
	@echo SRC_LIST=$(SRC_LIST)
	@echo OBJ_LIST=$(OBJ_LIST)
	@echo CC=$(CC)
	@echo FLAG=$(FLAG)

clean:$(CLEAN_LIST) 
	@echo --------------------------------------------------------------
	-rm -f $(TARGET) $(ARC_LIST) $(OBJ_LIST)
	-rm -f $(DPF)
	@echo

install:$(TARGET)
	@echo -------------------------install----------------------------
	cp $(TARGET)	/usr/bin/  
	rm /usr/etc/tsf/format -rf
	cp format /usr/etc/tsf/ -r

uninstall:
	@echo -------------------------uninstall--------------------------
	rm /usr/bin/$(TARGET) -f
	rm /usr/etc/tsf/format -rf



include ../common.mk
