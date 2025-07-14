// Simulation mehrerer Massen mit Gravitationsanziehung (2D) in SDL2 mit Spuren und Randreflexion
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>

const int WIDTH = 1200;
const int HEIGHT = 1000;
const float MAXSPEED = 20.0;

template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

struct Vector2 {
    float x, y;

    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }
    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }
    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }
    Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y;
        return *this;
    }
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    Vector2 normalized() const {
        float len = length();
        return len > 0 ? Vector2{x / len, y / len} : Vector2{0, 0};
    }
};

struct Body {
    Vector2 position;
    Vector2 velocity;
    float mass;
    SDL_Color color;
    void updateColorFromSpeed() {
        // Berechne Geschwindigkeit (Länge des Vektors)
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

        // Normiere Geschwindigkeit in Bereich [0.0, 1.0] – z. B. maxSpeed = 5.0
        float t = clamp(speed / MAXSPEED, 0.0f, 1.0f);

        // Interpolation von langsam (rot, dunkel) zu schnell (blau, hell)
        // Rot:   (R=255, G=0,   B=0)   bei t = 0
        // Blau:  (R=0,   G=128, B=255) bei t = 1
        Uint8 r = static_cast<Uint8>((1.0f - t) * 255);
        Uint8 g = static_cast<Uint8>(t * 255);
        Uint8 b = static_cast<Uint8>(t * 255);
        Uint8 a = 255;

        color = SDL_Color{r, g, b, a};
    }
};

float randomFloatMinusHalfToHalf() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
    return dist(gen);
}

std::vector<int> computeHistogram(const std::vector<Body>& bodies, float maxSpeed, int binCount) {
    std::vector<int> histogram(binCount, 0);

    for (const auto& body : bodies) {
        float speed = std::sqrt(body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y);
        int bin = std::min(static_cast<int>((speed / maxSpeed) * binCount), binCount - 1);
        histogram[bin]++;
    }

    return histogram;
}

void drawBody(SDL_Renderer* renderer, const Body& body) {
    // Körper zeichnen
    SDL_SetRenderDrawColor(renderer, body.color.r, body.color.g, body.color.b, 255);
    SDL_Rect rect = {(int)(body.position.x - 1), (int)(body.position.y - 1), 2, 2};
    SDL_RenderFillRect(renderer, &rect);
}

void drawHistogram(SDL_Renderer* renderer, const std::vector<int>& histogram, int x, int y, int width, int height) {
    int binCount = histogram.size();
    int binWidth = width / binCount;
    int maxValue = *std::max_element(histogram.begin(), histogram.end());

    if (maxValue == 0) return;

    for (int i = 0; i < binCount; ++i) {
        int barHeight = static_cast<int>((histogram[i] / static_cast<float>(maxValue)) * height);
        SDL_Rect rect = {
            x + i * binWidth,
            y + height - barHeight,
            binWidth - 1, // 1px spacing
            barHeight
        };

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 5); // hellblau
        SDL_RenderFillRect(renderer, &rect);
    }
}

void update(SDL_Renderer* renderer, const std::vector<Body>& bodies) {
    const int histogramHeight = 100;

    int binCount = 80;

    std::vector<int> histogram = computeHistogram(bodies, MAXSPEED, binCount);
    drawHistogram(renderer, histogram, 0, HEIGHT - histogramHeight, WIDTH, histogramHeight);
}

int main(int argc, char* argv[]) {

    int bodies_count = 1000;
    float G = 0.5f;   // Gravitationskonstante

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " bodies_count, gravitation\n";
        return 1;
    } 

    try {
        bodies_count = std::stoi(argv[1]);
        G = std::stof(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Invalid input: " << e.what() << "\n";
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mehr-Massen-Simulation mit Spuren", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::srand(std::time(nullptr));
    
    std::vector<Body> bodies = {
        {{500,500},{0,0},100,{255,255,255,255}}
    };

    for(int i=0; i<bodies_count; i++ ){
        float vx = randomFloatMinusHalfToHalf();
        float vy = randomFloatMinusHalfToHalf();
        Body b2;
        b2.position = Vector2{float(std::rand()%WIDTH), float(std::rand()%HEIGHT)};
        b2.velocity = Vector2{vx, vy};
        b2.mass = 5;
        b2.color = SDL_Color{255, 0, 0, 255};
        bodies.push_back(b2);
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Kräfte berechnen
        std::vector<Vector2> accelerations(bodies.size());
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = 0; j < bodies.size(); ++j) {
                if (i == j) continue;
                Vector2 dir = bodies[j].position - bodies[i].position;
                float dist = std::max(dir.length(), 10.0f);
                Vector2 forceDir = dir.normalized();
                float forceMag = G * bodies[i].mass * bodies[j].mass / (dist * dist);
                Vector2 acc = forceDir * (forceMag / bodies[i].mass);
                accelerations[i] += acc;
            }
        }

        // Bewegung aktualisieren mit Randreflexion
        for (size_t i = 0; i < bodies.size(); ++i) {
            bodies[i].velocity += accelerations[i];
            bodies[i].position += bodies[i].velocity;

            // Randreflexion
            if (bodies[i].position.x < 5 || bodies[i].position.x > WIDTH - 5) {
                bodies[i].velocity.x *= -1;
                bodies[i].position.x = clamp(bodies[i].position.x, 5.0f, (float)(WIDTH - 5));
            }
            if (bodies[i].position.y < 5 || bodies[i].position.y > HEIGHT - 5) {
                bodies[i].velocity.y *= -1;
                bodies[i].position.y = clamp(bodies[i].position.y, 5.0f, (float)(HEIGHT - 5));
            }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& body : bodies) {
            body.updateColorFromSpeed();
            drawBody(renderer, body);
        }

        update(renderer, bodies);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
