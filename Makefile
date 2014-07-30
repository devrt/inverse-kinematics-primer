CXXFLAGS = `rtm-config --cflags` `pkg-config --cflags opencv` `pkg-config --cflags hrpsys-base`
LDFLAGS  = `rtm-config --libs` `pkg-config --libs opencv`
SHFLAGS  = -shared

IDLC     = `rtm-config --idlc`
IDLFLAGS = `rtm-config --idlflags` -I`rtm-config --prefix`/include/rtm/idl
WRAPPER  = rtm-skelwrapper
WRAPPER_FLAGS = --include-dir="" --skel-suffix=Skel --stub-suffix=Stub

.SUFFIXES : .so

all: PIDController.so InverseKinematics.so HandMotionGeneration.so

.cpp.o:
	rm -f $@
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.o.so:
	rm -f $@
	$(CXX) $(SHFLAGS) -o $@ $^ $(LDFLAGS)

clean: clean_objs clean_skelstub
	rm -f *~

clean_objs:
	rm -f $(OBJS) *.o *.so

clean_skelstub:
	rm -f *Skel.h *Skel.cpp
	rm -f *Stub.h *Stub.cpp

PIDController.so: PIDController.o
PIDController.o: PIDController.h PIDController.cpp

InverseKinematics.so: InverseKinematics.o
InverseKinematics.o: InverseKinematics.h InverseKinematics.cpp

HandMotionGeneration.so: HandMotionGeneration.o
HandMotionGeneration.o: HandMotionGeneration.h HandMotionGeneration.cpp

# end of Makefile
