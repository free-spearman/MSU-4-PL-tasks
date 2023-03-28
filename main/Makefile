# Индентация осуществляется исключительно при помощи символов табуляции,
# каждой команде должен предшествовать отступ
#<цели>: <реквизиты>
#	<команда #1>
#	...
#	<команда #n>
.PHONY: all clean install uninstall


Flight.o: Flight.cpp
	g++ -c -o Flight.o Flight.cpp 
custom_types.o: custom_types.cpp
	g++ -c -o  custom_types.o custom_types.cpp
Graph.o: Graph.cpp
	g++ -c -o Graph.o Graph.cpp
DataReader.o: DataReader.cpp
	g++ -o DataReader.o -c DataReader.cpp
SearchAlgorithms.o: SearchAlgorithms.cpp
	g++ -o SearchAlgorithms.o -c SearchAlgorithms.cpp  
main.o:
	g++ -o main.o -c main.cpp
main: main.o DataReader.o Graph.o custom_types.o Flight.o
	g++ -o main custom_types.o Flight.o Graph.o DataReader.o main.o     

clean:
	rm -rf *.o
	rm main
