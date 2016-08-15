all: ir2x10
ir2x10:  x10_tx.o  ir2x10.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -lrt -llirc_client 
clean:
	$(RM) *.o
