CXX = g++.exe
SFML_PATH = D:\Programming\SFML-2.6.2
GLEW_PATH = D:\Programming\glew-2.2.0
ASSIMP_PATH = D:\Programming\assimp
GLM_PATH = D:\Programming\glm-1.0.1-light


FLAG1 = -I$(SFML_PATH)\include -Iinclude -I$(GLM_PATH) -I$(GLEW_PATH)\include -I$(ASSIMP_PATH)\include -Wall
FLAG_RELEASE = -L$(SFML_PATH)\lib -lsfml-graphics -lsfml-window -lsfml-system -mwindows -lopengl32 -L$(GLEW_PATH)\lib\Release\Win32 -lglew32 -L$(ASSIMP_PATH)\lib -llibassimp
FLAG_DEBUG = -L$(SFML_PATH)\lib -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lopengl32 -L$(GLEW_PATH)\lib\Release\Win32 -lglew32 -L$(ASSIMP_PATH)\lib -llibassimp

all: release debug

release: release/Main.exe src/gl.o move
release/Main.exe: src/Main.o
	$(CXX) $^ -o $@ $(FLAG_RELEASE)
	copy $(SFML_PATH)\bin\*.dll release\ /Y
	copy $(GLEW_PATH)\bin\Release\Win32\*.dll release /Y

debug: debug/Main.exe
debug/Main.exe: src/Main.o src/gl.o
	$(CXX) $^ -o $@ $(FLAG_DEBUG)
	copy $(SFML_PATH)\bin\*.dll debug\ /Y
	copy $(GLEW_PATH)\bin\Release\Win32\*.dll debug /Y

# copy all the dll, it is simple, but not every dll is needed.

src/Main.o: src/Main.cpp
	$(CXX) -c $^ -o $@ $(FLAG1)
src/gl.o: src/gl.cpp
	$(CXX) -c $^ -o $@ $(FLAG1)

move:
	xcopy assets release\assets /E /I /Y


clean:
	del release\Main.exe debug\Main.exe /Q
	del src\*.o /Q