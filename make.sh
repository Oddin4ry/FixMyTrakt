
	echo "Creating folders"
	mkdir ./source/
	mkdir ./bin/
	echo "Folders created"

	echo "Removing old main"
	rm ./bin/FixMyTrakt
	echo Compiling Main
	g++ ./source/main.h ./source/FixMyTrakt_main.cc ./source/jsonbuilder.cpp ./source/ITraktTask.cpp ./source/CleanRatingsTask.cpp ./source/FixMyTrakt_manager.cc ./source/ClassLogger.cpp ./source/DBManager.cc ./source/FixMyTrakt_httpstraktmanager.cc -ljsoncpp -l curl -o ./bin/FixMyTrakt `pkg-config gtkmm-3.0 --cflags --libs`

	echo "main compiled"
	echo "Will try to execute"
	./bin/FixMyTrakt
	echo "Finished"
