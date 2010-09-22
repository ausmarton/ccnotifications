#!/bin/bash

function clean() {
	rm notifications
}

function build() {
	gcc -Wall -o notifications test-notify.c `pkg-config --libs --cflags glib-2.0 gtk+-2.0` -lnotify
}

function run() {
	./notifications
}

if [ "$1" == "clean" ]; then
	clean
elif [ "$1" == "build" ]; then
	clean
	build
else
	clean
	build
	run
fi
