CC = g++
CFLAGS = -O0 -g3 -Wall -c -ggdb `pkg-config --cflags opencv` -mmacosx-version-min=10.8
LIBS =    -L/usr/lib -lpthread -ldl -lm -std=gnu++0x -std=c++0x \
          -lopencv_core -lopencv_imgproc -lopencv_highgui        \
          -lopencv_ml -lopencv_video -lopencv_features2d         \
          -lopencv_calib3d -lopencv_objdetect -lopencv_flann

SOURCES = ./camera/camera.cpp
OBJECTS = balanceball.o
EXECS   = balanceball

$(EXECS): $(SOURCES)
	@echo ‘**********************’
	@echo 'Building...'
	$(CC) $(SOURCES) -o $(EXECS) $(FLAGS) $(LIBS)
	@echo 'Success!'
	@echo ‘**********************’

