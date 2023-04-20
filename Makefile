# Индентация осуществляется исключительно при помощи символов табуляции,
# каждой команде должен предшествовать отступ
#<цели>: <реквизиты>
#	<команда #1>
#	...
#	<команда #n>
CC_SRC_FOLDER = src_cpp
CC_OBJ_FOLDER = build
CXX=g++
CXX_FLAGS=-O3 -std=c++17
LIBS= -lncursesw
TARGET_WITHOUT_LINK=$(CC_SRC_FOLDER)/$@.cpp
CXX_WITHOUT_LINK=$(CXX) -c -o $(CC_OBJ_FOLDER)/$@.o $^ $(CXX_FLAGS)
NAME=program
REQ_NAME=BaseLogger Flight custom_types Graph SearchAlgorithms main Susanin    
#REQ:=$(addprefix $(CC_OBJ_FOLDER)/, $(REQ_NAME))
REQ=$(addsuffix .o,$(addprefix $(CC_OBJ_FOLDER)/, $(REQ_NAME))) 
.PHONY: all clean install uninstall

print:
	echo $(REQ)
	echo $(REQ_NAME)
build_dir:
	mkdir $(CC_OBJ_FOLDER) 

BaseLogger: $(CC_SRC_FOLDER)/BaseLogger.cpp
	$(CXX_WITHOUT_LINK)
Flight: $(CC_SRC_FOLDER)/Flight.cpp
	$(CXX_WITHOUT_LINK)
custom_types: $(CC_SRC_FOLDER)/custom_types.cpp
	$(CXX_WITHOUT_LINK)
Graph: $(CC_SRC_FOLDER)/Graph.cpp
	$(CXX_WITHOUT_LINK)
DataReader: DataReader.cpp
	$(CXX_WITHOUT_LINK)
SearchAlgorithms: $(CC_SRC_FOLDER)/SearchAlgorithms.cpp
	$(CXX_WITHOUT_LINK)
Susanin: $(CC_SRC_FOLDER)/Susanin.cpp
	$(CXX_WITHOUT_LINK)
test: test.cpp BaseLogger.o
	$(CXX_WITHOUT_LINK)
main:$(CC_SRC_FOLDER)/main.cpp
	$(CXX_WITHOUT_LINK)
build: build_dir $(REQ_NAME)  
	$(CXX) $(CXX_FLAGS) $(REQ)  $(LIBS) -o $(NAME) 
rebuild: $(REQ)
	$(CXX) $(CXX_FLAGS) $(REQ)  $(LIBS) -o $(NAME)
run: $(NAME)
	./$(NAME)
linux: main.cpp BaseLogger.cpp Flight.cpp Graph.cpp custom_types.cpp SearchAlgorithms.cpp Susanin.cpp
	$(CXX) $(CXX_FLAGS)  $^ $(LIBS) -o $@
clean:
	rm -r $(CC_OBJ_FOLDER)
	rm $(NAME)
	
