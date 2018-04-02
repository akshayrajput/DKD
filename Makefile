#OBJS specifies which files to compile as part of the project 
OBJS = Util.cpp Texture.cpp Server.cpp Map.cpp Hero.cpp main.cpp
#CC specifies which compiler we're using 
CC = g++ -std=c++11 -Wall
#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
COMPILER_FLAGS = -Wall -g  
#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lGL -lGLU -lglut -lpng -lpthread
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = DkD
#This is the target that compiles our executable 
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm -rf OBJ_NAME *.o

