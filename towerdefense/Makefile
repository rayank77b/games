CXX       := g++
CXXFLAGS  := -std=c++14 -Wall `sdl2-config --cflags`
CXXFLAGS  += -I/usr/include/SDL2 `pkg-config --cflags SDL2_ttf`
LDFLAGS   := `sdl2-config --libs` -lSDL2_ttf

SRC_DIR   := src
SRCS      := $(SRC_DIR)/main.cpp \
             $(SRC_DIR)/Game.cpp \
             $(SRC_DIR)/Bullet.cpp \
             $(SRC_DIR)/Enemy.cpp \
             $(SRC_DIR)/Tower.cpp
OBJS      := $(SRCS:.cpp=.o)
DEPS      := $(OBJS:.o=.d)

TARGET    := tower_defense

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# erstellt .o und .d (Dependencies)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

# lade automatisch die Abhängigkeiten aus den .d-Dateien
-include $(DEPS)