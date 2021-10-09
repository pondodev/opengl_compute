executable_name = output

full: build run

fullclean: clean build run

build:
		g++ \
		-lglfw \
		-lGL \
		-lX11 \
		-lpthread \
		-lXrandr \
		-lXi \
		-ldl \
		-I ./include/ \
		-o $(executable_name) glad.c *.cpp

run:
		./$(executable_name)

clean:
		rm -f ./$(executable_name)