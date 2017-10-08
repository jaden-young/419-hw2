DISTDIR=./dist/

.PHONY: clean

all: doc

graph:
	g++ -std=c++11 -O3 -w assign2Graph.cpp -lpthread -o assign2Graph

bench: graph
	python3 run_bench.py

doc: bench
	emacs readme.org --batch -f org-latex-export-to-pdf --kill

clean:
	rm -rf assign2Graph networkDatasets/HcNetwork_bench.csv *.tex* __pycache__ \
				 auto *.pdf dist/

$(DISTDIR):
	mkdir -v -p $(DISTDIR)

dist: $(DISTDIR)
	tar --exclude='dist' --exclude='Assignment2.pdf' -zcvf ./dist/jadyoungAssignment2.tgz *
