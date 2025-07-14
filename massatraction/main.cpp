// Simulation mehrerer Massen mit Gravitationsanziehung (2D) in SDL2 mit Spuren und Randreflexion
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <cxxopts.hpp>

const int WIDTH = 1200;
const int HEIGHT = 1000;
const float MAXSPEED = 20.0;

const int histogramHeight = 100;
const int binCount = 80;

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
    float distance(const Vector2& v) const {
        return std::sqrt((v.x-x) * (v.x-x) + (v.y-y) * (v.y-y));
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
        float speed = velocity.length();

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

std::vector<int> computeHistogram(const std::vector<Body>& bodies, const float maxSpeed, const int binCount) {
    std::vector<int> histogram(binCount, 0);

    for (const auto& body : bodies) {
        float speed = body.velocity.length();
        int bin = std::min(static_cast<int>((speed / maxSpeed) * binCount), binCount - 1);
        histogram[bin]++;
    }

    return histogram;
}

void drawBody(SDL_Renderer* renderer, const Body& body) {
    // Körper zeichnen
    if(body.position.x>=0 && body.position.y>=0 && body.position.x<=WIDTH && body.position.y<=HEIGHT) {
        SDL_SetRenderDrawColor(renderer, body.color.r, body.color.g, body.color.b, 255);
        SDL_Rect rect = {(int)(body.position.x - 1), (int)(body.position.y - 1), 2, 2};
        SDL_RenderFillRect(renderer, &rect);
    }
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

void removeCloseBodies(std::vector<Body>& bodies, float minDistance) {
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); /* kein ++ */) {
            float dist = bodies[i].position.distance(bodies[j].position);
            if (dist < minDistance) {
                float totalMass = bodies[i].mass + bodies[j].mass;
                Vector2 newVelocity = {
                    (bodies[i].velocity.x * bodies[i].mass + bodies[j].velocity.x * bodies[j].mass) / totalMass,
                    (bodies[i].velocity.y * bodies[i].mass + bodies[j].velocity.y * bodies[j].mass) / totalMass
                };
                bodies[i].mass = totalMass;
                bodies[i].velocity = newVelocity;
                bodies.erase(bodies.begin() + j); // entferne b
            } else {
                ++j;
            }
        }
    }
}

Vector2 computeTotalMomentum(const std::vector<Body>& bodies) {
    Vector2 total = {0.0f, 0.0f};

    for (const auto& body : bodies) {
        total.x += body.velocity.x * body.mass;
        total.y += body.velocity.y * body.mass;
    }

    return total;
}

int main(int argc, char* argv[]) {

    cxxopts::Options options("massatraction", "A mass atraction simulation");

    options.add_options()
        ("n,number", "Number of mass", cxxopts::value<int>()->default_value("1000"))
        ("g,gravity", "Gravity constant", cxxopts::value<float>()->default_value("0.5"))
        ("r,reflexion", "switch reflexion on border on", cxxopts::value<bool>()->default_value("false"))
        ("i,histogramm", "show histogramm", cxxopts::value<bool>()->default_value("false"))
        ("a,addmassiv", "add a massive object in center (number is the mass of the object)", cxxopts::value<int>()->default_value("10"))
        ("m,merge", "switch on that mass will be merged", cxxopts::value<bool>()->default_value("false"))
        ("d,mindistance", "mindistance it should have to calculate",cxxopts::value<float>()->default_value("10"))
        ("h,help", "Show help");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help();
        return 0;
    }

    int bodies_count = result["number"].as<int>();
    float G = result["gravity"].as<float>(); 
    bool reflexion_on = result["reflexion"].as<bool>();
    bool histogramm_on = result["histogramm"].as<bool>();
    int massive_mass = result["addmassiv"].as<int>();
    float mindistance = result["mindistance"].as<float>(); 
    bool mass_merge = result["merge"].as<bool>();

    std::cout<<"start with bodies count: "<<bodies_count<<std::endl;
    std::cout<<"min distance to calculate: "<<mindistance<<std::endl;
    if(mass_merge)
        std::cout<<"mass merge is activated"<<std::endl;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mehr-Massen-Simulation mit Spuren", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::srand(std::time(nullptr));
    
    std::vector<Body> bodies;

    Body bmassiv;
    bmassiv.position = Vector2{WIDTH/2.0, HEIGHT/2.0};
    bmassiv.velocity = Vector2{0.0, 0.0};
    bmassiv.mass = massive_mass;
    bmassiv.color = SDL_Color{255, 255, 255, 255};
    bodies.push_back(bmassiv); 

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
        
        Vector2 p = computeTotalMomentum(bodies);
        float magnitude = std::sqrt(p.x * p.x + p.y * p.y);
        std::cout<<"\rbodies count: "<<bodies.size();
        std::cout<<"  Gesamtimpuls: "<< magnitude<<std::flush;
        // berechne ob massen zueiander zu nah sind und füge sie zusammen
        if(mass_merge)
            removeCloseBodies(bodies, mindistance);

        // Kräfte berechnen
        std::vector<Vector2> accelerations(bodies.size());
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = 0; j < bodies.size(); ++j) {
                if (i == j) continue;
                Vector2 dir = bodies[j].position - bodies[i].position;
                float dist = std::max(dir.length(), 5.0f); //mindistance);
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

            if(reflexion_on) {
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

        }

        // delete the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw bodies
        for (auto& body : bodies) {
            body.updateColorFromSpeed();
            drawBody(renderer, body);
        }

        // draw histogram
        if(histogramm_on) {
            std::vector<int> histogram = computeHistogram(bodies, MAXSPEED, binCount);
            drawHistogram(renderer, histogram, 0, HEIGHT - histogramHeight, WIDTH, histogramHeight);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
