


TARGET:= ../main/lib/expat.a

SRC_LIST:= xmlparse.c xmlrole.c xmltok.c xmltok_impl.c xmltok_ns.c

OBJ_LIST:= $(patsubst %.c, obj/%.o,$(SRC_LIST))

INCLUDE:= 
# -I../portable/ -I../utility/

$(TARGET): $(OBJ_LIST)
	ar -r $@ $^

$(OBJ_LIST):obj/%.o:%.c
	$(CC) $(FLAG) $(INCLUDE) -c $< -o $@

test:
	@echo TARGET=$(TARGET)
	@echo SRC_LIST=$(SRC_LIST)
	@echo OBJ_LIST=$(OBJ_LIST)
	@echo INCLUDE=$(INCLUDE)
	@echo CC=$(CC)
	@echo FLAG=$(FLAG)

clean:
	-rm -f $(TARGET) $(OBJ_LIST)

include ../common.mk