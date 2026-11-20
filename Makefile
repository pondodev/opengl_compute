executable_name = output

full: build run

fullclean: clean build run

build:
		g++ \
		 glad.c *.cpp \
		-I ./include/ \
		-lglfw \
		-lGL \
		-lX11 \
		-lpthread \
		-lXrandr \
		-lXi \
		-ldl \
		-o $(executable_name)

run:
		./$(executable_name)

clean:
		rm -f ./$(executable_name)
