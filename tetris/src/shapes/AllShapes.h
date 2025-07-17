#pragma once
#include "../Tetromino.h"

// Jede Form erhält eigene Farbe + Start-Blocks (relativ zum Pivot (0,0))

class IShape : public Tetromino {
public:
    IShape() : Tetromino({0,255,255,255}) {
        blocks_ = { {-2,0}, {-1,0}, {0,0}, {1,0} };
    }
};
class JShape : public Tetromino {
public:
    JShape() : Tetromino({0,0,255,255}) {
        blocks_ = { {-1,0}, {0,0}, {1,0}, {1,1} };
    }
};
class LShape : public Tetromino {
public:
    LShape() : Tetromino({255,165,0,255}) {
        blocks_ = { {-1,0}, {0,0}, {1,0}, {-1,1} };
    }
};
class OShape : public Tetromino {
public:
    OShape() : Tetromino({255,255,0,255}) {
        blocks_ = { {0,0}, {1,0}, {0,1}, {1,1} };
    }
    void rotate() override {}  // O-Block rotiert nicht
};
class SShape : public Tetromino {
public:
    SShape() : Tetromino({0,255,0,255}) {
        blocks_ = { {-1,1}, {0,1}, {0,0}, {1,0} };
    }
};
class TShape : public Tetromino {
public:
    TShape() : Tetromino({160,32,240,255}) {
        blocks_ = { {-1,0}, {0,0}, {1,0}, {0,1} };
    }
};
class ZShape : public Tetromino {
public:
    ZShape() : Tetromino({255,0,0,255}) {
        blocks_ = { {-1,0}, {0,0}, {0,1}, {1,1} };
    }
};
