#	FILE			: makefile
#	PROJECT			: SP_A03
#	PROGRAMMER		: Mita Das & Colby Taylor
#	FIRST VERSION		: 2022-02-21
#	DESCRIPTION		: this makefile will compile the data corruptor code
#
#	AUTHOR			: Sean Clark
#	SOURCE			: https://conestoga.desire2learn.com/d2l/le/content/534837/viewContent/11113833/View
#
# 
# =======================================================
#                  Common Application
# =======================================================
# FINAL BINARY Target
./bin/dataCorruptor : ./obj/dataCorruptor.o
	cc ./obj/dataCorruptor.o -o ./bin/dataCorruptor
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/dataCorruptor.o : ./src/dataCorruptor.c ./inc/dataCorruptor.h
	cc -c ./src/dataCorruptor.c -o ./obj/dataCorruptor.o

#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/dataCorruptor

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


