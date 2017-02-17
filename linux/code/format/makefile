

TARGET:= ../main/lib/format.a

SRC_LIST:= formatdb.cpp formattree.cpp formatparser.cpp formatserver.cpp formatfile.cpp\
	tableformat.cpp sectionformat.cpp dvbdescriptorformat.cpp mpeg2descriptorformat.cpp\
	packetformat.cpp

OBJ_LIST:= $(patsubst %.cpp, obj/%.o,$(SRC_LIST))

INCLUDE:= -I../portable/ -I../dvb/ -I../utility -I../content -I../expat

$(TARGET): $(OBJ_LIST)
	ar -r $@ $^

$(OBJ_LIST):obj/%.o:%.cpp
	$(CC) $(FLAG)  $(INCLUDE) -c $< -o $@

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