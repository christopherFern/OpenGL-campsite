TARGET=example4

$(TARGET) : main.o Shaders.o tiny_obj_loader.o
	c++ -o $(TARGET) main.o Shaders.o tiny_obj_loader.o -lglfw -lGLEW -lGLU -lGL -lm

main.o: Shaders.h tiny_obj_loader.h

Shaders.o: Shaders.h

tiny_obj_loader.o: tiny_obj_loader.h

clean:
	rm $(TARGET) main.o Shaders.o tiny_obj_loader.o
