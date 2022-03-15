#	FILE			: makefile
#	PROJECT			: SP_A03
#	PROGRAMMER		: Mita Das & Colby Taylor
#	FIRST VERSION		: 2022-02-21
#	DESCRIPTION		: this makefile will compile the common code
#
#	AUTHOR			: Sean Clark
#	SOURCE			: https://conestoga.desire2learn.com/d2l/le/content/534837/viewContent/11113833/View
#
# 
# =======================================================
#                  Common Application
# =======================================================
# FINAL BINARY Target
./bin/common : ./obj/common.o
	cc ./obj/common.o -o ./bin/common
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/common.o : ./src/common.c ./inc/common.h
	cc -c ./src/common.c -o ./obj/common.o

#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/common

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	


