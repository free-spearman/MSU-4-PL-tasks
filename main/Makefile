# Индентация осуществляется исключительно при помощи символов табуляции,
# каждой команде должен предшествовать отступ
#<цели>: <реквизиты>
#	<команда #1>
#	...
#	<команда #n>
.PHONY: all clean install uninstall


Flight.o: Flight.cpp
	g++ -c -o flight.o flight.cpp 
custom_class.o: custom_class.cpp
	g++ -c -o custom_class.o custom_class.cpp
Graph.o: Graph.cpp
	g++ -c -o Graph.o Graph.cpp
DataReader.o: DataReader.cpp
	g++ -c -o DataReader.o DataReader.cpp 
clean:
	rm -rf *.o
