# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $

all:
	cd ./gezel;     make ; cd .
	cd ./gplatform; make ; cd .
	cd ./ipblock;   make ; cd .

sim:
	cd ./gezel;     make sim; cd .
	cd ./gplatform; make sim; cd .
	cd ./ipblock;   make sim; cd .

clean:
	cd ./gezel;     make clean; cd .
	cd ./gplatform; make clean; cd .
	cd ./ipblock;   make clean; cd .

xtra:
	cd ./systemc;   make ; make sim; make clean; cd .
	cd ./java;      make ; make sim; make clean; cd .
