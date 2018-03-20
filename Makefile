src=$(wildcard ./*.cpp)
headers=$(wildcard ../*.h)


cflags=-g -Wall -std=c++11
ldflags=

progname=calc

obj=$(wildcard *.o)

$(progname):$(src) $(headers)
	g++ $(cflags) -o $@ $(src) $(ldflags)
	

clean:
	rm -f $(progname) $(progname).* $(obj)
	
run:
	./$(progname)