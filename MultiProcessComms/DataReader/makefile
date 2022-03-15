#	FILE			: makefile
#	PROJECT			: SP_A03
#	PROGRAMMER		: Mita Das & Colby Taylor
#	FIRST VERSION	: 2022-02-21
#	DESCRIPTION		: this makefile will compile the data reader code
#
#	AUTHOR			: Sean Clark
#	SOURCE			: https://conestoga.desire2learn.com/d2l/le/content/534837/viewContent/11113833/View
#
# 
# =======================================================
#                  Data Reader Application
# =======================================================
# FINAL BINARY Target
./bin/dataReader : ./obj/dataReader.o
	cc ./obj/dataReader.o -o ./bin/dataReader
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/dataReader.o : ./src/dataReader.c ./inc/dataReader.h
	cc -c ./src/dataReader.c -o ./obj/dataReader.o

#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/dataReader

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


