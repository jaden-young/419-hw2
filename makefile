DISTDIR=./dist/

.PHONY: clean dist

all: graph

graph: assign2Graph.cpp
	g++ -std=c++11 -O3 -w assign2Graph.cpp -lpthread -o assign2Graph

bench: graph
	python3 run_bench.py

pdf: graph
	yes 'yes' | emacs readme.org --batch --eval "(setq org-babel-python-command \"python3\")" --eval "(require (quote ob-python))" -f org-latex-export-to-pdf --kill

clean:
	rm -rf  *.tex* __pycache__ auto

distclean: clean
	rm -rf *~ networkDatasets/HcNetwork_bench.csv $(DISTDIR) readme.pdf assign2Graph

$(DISTDIR):
	mkdir -v -p $(DISTDIR)

dist: readme.org lab_bench.sh run_bench.py $(DISTDIR)
	tar --exclude='dist' --exclude='Assignment2.pdf' -zcvf $(DISTDIR)/jadyoungAssig2.tgz *
