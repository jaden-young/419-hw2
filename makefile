all:graph

graph:
	g++ -O3 -w assign2Graph.cpp -lpthread -o assign2Graph

clean:
	rm assign2Graph

dist:graph
	tar --exclude='dist' --exclude='Assignment2.pdf' -zcvf ./dist/jadyoungAssignment2.tgz *
