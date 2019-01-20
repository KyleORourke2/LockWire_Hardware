CC := g++
LDFLAGS := -lupm-mma7660
CXXFLAGS := -Wextra -g

all:LockWire

accel:
	@$(CC) $(CFLAGS) src/LockWire.cpp -o accel $(LDFLAGS)
clean:
	@rm -f ./LockWire
