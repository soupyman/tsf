
TARGET:= ../main/lib/portable.a

SRC_LIST= file.cpp cstring.cpp

OBJ_LIST=$(patsubst %.cpp, obj/%.o,$(SRC_LIST))

$(TARGET): $(OBJ_LIST)
	ar -r $@ $^

$(OBJ_LIST):obj/%.o:%.cpp
	$(CC) $(FLAG)  $(INCLUDE) -c $< -o $@

test:
	@echo TARGET=$(TARGET)
	@echo SRC_LIST=$(SRC_LIST)
	@echo OBJ_LIST=$(OBJ_LIST)
	@echo CC=$(CC)
	@echo FLAG=$(FLAG)

clean:
	-rm -f $(TARGET) $(OBJ_LIST)


include ../common.mk
