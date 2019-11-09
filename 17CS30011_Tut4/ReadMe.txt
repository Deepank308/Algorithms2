- This Directory is the complete implementation of Assignment no. 4.

- Structure of the Directory:

--- include/
        Header files for data structures(DCEL, Beachline), Auxiliary utility structures(Geometry, Point2D), Voronoi diagram.
--- src/
        source code implementation of the classes mentioned in Header files.
        main.cpp
--- Makefile
        $ make --> compiles the main.cpp file with C++14 required standard for GNU GCC.
        $ make run --> exectues the generated main.out file and outputs a SVG file.
        $ make clean --> remove main.out, if exists.
--- t4.svg
        Generated output from the main.cpp file.

- Credits:
    Deepank Agrawal 17CS30011
    GitHub ID: Deepank308

Note: Some references has been taken from various C++ implementations available on GitHub to inculcate good SD practices.
