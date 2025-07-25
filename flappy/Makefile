CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -D_REENTRANT -MMD -MP
CXXFLAGS  += -I/usr/include/SDL3 -I/usr/include/SDL_image -I/usr/include/SDL3_ttf 
LDFLAGS   := -lSDL3 -lSDL3_ttf

SRC_DIR   := src
SRCS      := $(wildcard $(SRC_DIR)/*.cpp)
OBJS      := $(SRCS:.cpp=.o)
DEPS      := $(OBJS:.o=.d)

TARGET    := flappy

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
