#!/bin/sh
chmod +x ./socket.sh
gcc server.c -o s.out
./s.out &
gcc client.c -o c.out
./c.out &
./c.out &
./c.out &
./c.out &
./c.out &
