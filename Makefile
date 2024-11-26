all: build 

build: differenciator

differenciator: 
	cd Differenc && make

run: 
	cd Differenc && ./Differenciator

clean: 
	cd Differenc && make clean
		