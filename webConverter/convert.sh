#!/bin/bash

main() {
    filename=$(basename "$1")
    gzip -k $1
    result=$(cat $filename.gz | hexdump -ve '1/1 "0x%.2x, "')
    nameprog=$(echo "$filename" | sed 's/\.//g')
    echo "const char ${nameprog}[] PROGMEM = {$result};" > data.h
    rm -rf $filename.gz
}
main $1