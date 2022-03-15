#	FILE			: makefile
#	PROJECT			: SP_A03
#	PROGRAMMER		: Mita Das & Colby Taylor
#	FIRST VERSION		: 2022-02-21
#	DESCRIPTION		: this makefile will compile the data creator code
#
#	AUTHOR			: Sean Clark
#	SOURCE			: https://conestoga.desire2learn.com/d2l/le/content/534837/viewContent/11113833/View
#
# 
# =======================================================
#                  Data Creator Application
# =======================================================
# FINAL BINARY Target
./bin/dataCreator : ./obj/dataCreator.o
	cc ./obj/dataCreator.o -o ./bin/dataCreator
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/dataCreator.o : ./src/dataCreator.c ./inc/dataCreator.h
	cc -c ./src/dataCreator.c -o ./obj/dataCreator.o
#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/dataCreator

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


