#! /bin/sh

if ! [ -d "build" ]; then
    mkdir build
fi

if [ -f "build/clexer" ]; then
    rm build/clexer
fi

gcc main.c -o build/clexer -Wall

if [ "$?" ]; then
    build/clexer
fi
