all:
	g++ segment.cpp -o out `pkg-config --libs opencv`
	./out

clean:
	rm *.jpg
	rm out
