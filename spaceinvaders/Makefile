CXX       := g++
CXXFLAGS  := -std=c++14 -Wall `sdl2-config --cflags`
CXXFLAGS  += -MMD -MP
LDFLAGS   := `sdl2-config --libs` -lSDL2_ttf

SRC_DIR   := src
SRCS      := src/main.cpp \
             $(SRC_DIR)/Config.cpp \
             $(SRC_DIR)/Game.cpp \
             $(SRC_DIR)/Player.cpp \
             $(SRC_DIR)/Enemy.cpp \
             $(SRC_DIR)/Bullet.cpp
OBJS      := $(SRCS:.cpp=.o)
DEPS      := $(OBJS:.o=.d)

TARGET    := invaders

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
