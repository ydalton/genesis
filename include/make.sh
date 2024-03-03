#!/bin/sh

PREFIX=m68k-elf
AS=$PREFIX-as
LD=$PREFIX-ld
OBJCOPY=$PREFIX-objcopy
CC=$PREFIX-gcc

SRC=game.S
TARGET=game

if [ ! -z $1 ]; then
	if [ $1 = "clean" ]; then
		echo "  CLEAN ."
		rm -f $TARGET game.o
		exit 0
	fi
fi

TMP=$(mktemp)

cleanup () {
	rm -f $TMP
}
echo "  CC	game.o"
$CC -c -mcpu=68000 -I ../include -Xassembler --bitwise-or -o game.o $SRC
if [ $? -ne 0 ]; then
	cleanup
	exit 1
fi

$OBJCOPY -O binary game.o $TMP
if [ $? -ne 0 ]; then
	cleanup
	exit 1
fi
echo "  LD	$TARGET"
$LD -defsym __checksum=0x$(mdcksum $TMP) -T ../include/linker.lds game.o -o $TARGET
if [ $? -ne 0 ]; then
	cleanup
	exit 1
fi


