CC              =  g++
CFLAGS          = -Wall -g -O0
LIBS            = -lopencv_core         \
                  -lopencv_imgproc  	\
		  -lopencv_highgui	\
                  -lopencv_ml           \
                  -lopencv_video        \
                  -lopencv_features2d   \
                  -lopencv_calib3d      \
                  -lopencv_objdetect    \
                  -lopencv_flann


SOURCES         = balanceball.cpp 	   \
		  ./camera/camera.cpp	   \
	          ./pid/pid.cpp		   \
	          ./platform/platform.cpp

OBJECTS         = $(SOURCES:.c=.o)
EXECUTABLE      = balanceball


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "====================================================="
	@echo "Building BalanceBall"
	@echo ""

	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

	@echo ""
	@echo "Success"
	@echo "====================================================="

%.o: %.c
	$(CC) -c -MD $(CFLAGS) -o $@ $<

-include $(wildcard *.d)

valgrind:
	valgrind ./$(EXECUTABLE)

make_clean: all clean

clean:
	rm -f *.o *.d

clean_all:
	rm -f *.o *.d $(EXECUTABLE)



.PHONY: all clean

