


TARGET:= ../main/lib/dvb.a

SRC_LIST:= tsfile.cpp packet.cpp section.cpp table.cpp

OBJ_LIST:= $(patsubst %.cpp, obj/%.o,$(SRC_LIST))

INCLUDE:= -I../portable/ -I../utility/ -I../dvb/ -I../format/ -I../content/



$(TARGET): $(OBJ_LIST)
	ar -r $@ $^

$(OBJ_LIST):obj/%.o:%.cpp
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
