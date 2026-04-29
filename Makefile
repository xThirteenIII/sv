src_dir = src
build_dir = build
src_files = main.c process.c

run: $(build_dir)/main
	./$(build_dir)/main

$(build_dir)/main: $(build_dir)/main.o
	cc $(src_dir)/main.c -o $(build_dir)/main

$(build_dir)/main.o: $(src_dir)/main.c
	cc -c $(src_dir)/main.c -o $(build_dir)/main.o

$(src_dir)/main.c:
	echo "ciao"
