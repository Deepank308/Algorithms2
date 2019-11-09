CFlags = -Iinclude/ -std=c++14
Files = src/Point2D.cpp src/VisualizeSVG.cpp src/VoronoiDiagram.cpp src/Beachline.cpp src/DCEL.cpp src/Geometry.cpp

main: clean
	g++ $(CFlags) $(Files) src/main.cpp -o main

run:
	./main

clean:
	rm -f main
