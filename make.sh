CC = gcc

	echo "Creating folders"
	mkdir ./source/
	mkdir ./bin/
	echo "Folders created"

	echo "Removing old main"
	rm ./bin/FixMyTrakt
	echo Compiling Main
	g++ ./source/FixMyTrakt_main.cc -o ./bin/FixMyTrakt `pkg-config gtkmm-3.0 --cflags --libs`

	echo "main completed"
