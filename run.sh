#!/usr/bin/sh
clear;
set -xe;
g++ -c main.cpp && g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system && ./main;