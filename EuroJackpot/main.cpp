#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>

std::vector<int> generateNumbers(int count, int min, int max) {
    std::vector<int> pool;
    // erzeuge zahlen von min bis max
    for (int i = min; i <= max; ++i)
        pool.push_back(i);
    // initialisiere Random
    std::random_device rd;
    std::mt19937 gen(rd());
    // vermische die zahlen shuffle
    std::shuffle(pool.begin(), pool.end(), gen);

    // waehle erste count zahlne aus dem vector pool und liefere es als vector
    return std::vector<int>(pool.begin(), pool.begin() + count);
}

void simplePrint10Numbers() {
    std::cout << "EuroJackpot Zahlen:\n";
    for(int i=0; i<10; i++) {
        // 5 Hauptzahlen aus 1–50
        std::vector<int> mainNumbers = generateNumbers(5, 1, 50);
        std::sort(mainNumbers.begin(), mainNumbers.end());

        // 2 Eurozahlen aus 1–12
        std::vector<int> euroNumbers = generateNumbers(2, 1, 12);
        std::sort(euroNumbers.begin(), euroNumbers.end());

        std::cout << "Hauptzahlen: ";
        for (int n : mainNumbers)
            std::cout << n << " ";

        std::cout << "    Eurozahlen: ";
        for (int e : euroNumbers)
            std::cout << e << " ";

        std::cout << std::endl;
    }
}

std::vector<int> convert2vectorInt(char *argv) {
    std::string input = argv;
    std::vector<int> mainNumbers;
    std::stringstream ss(input);
    int number;

    // Mit >> number lesen, auf Komma prüfen und ggf. überspringen
    while (ss >> number) {
        mainNumbers.push_back(number);
        if (ss.peek() == ',')
            ss.ignore();  // Komma überspringen
    }

    // Ausgabe zur Kontrolle
    for (int x : mainNumbers) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    return mainNumbers;
}
int getRichtige(std::vector<int> G, std::vector<int> S) {
    int richtige = 0;
    for(auto g : G) 
        for(auto s: S)
            if(g==s)
                richtige++;
    return richtige;
}

float getGewin(std::vector<int> mainNumbers, std::vector<int> euroNumbers, std::vector<int> mainSpiel, std::vector<int> euroSpiel) {
    int richtigeMN = 0;
    int richtigeEN = 0;
    
    richtigeMN = getRichtige(mainNumbers, mainSpiel);
    richtigeEN = getRichtige(euroNumbers, euroSpiel);

    if(richtigeMN==5 && richtigeEN==2) { // jackpot
        return 97000000.0;
    }
    if(richtigeMN==5 && richtigeEN==1) { 
        return 550000.0;
    }
    if(richtigeMN==5 && richtigeEN==0) { // 
        return 220000.0;
    }
    if(richtigeMN==4 && richtigeEN==2) { // 
        return 5000.0;
    }
    if(richtigeMN==4 && richtigeEN==0) { // 
        return 100.0;
    }
    if(richtigeMN==3 && richtigeEN==2) { // 
        return 140.0;
    }
    if(richtigeMN==3 && richtigeEN==1) { // 
        return 18.0;
    }
    if(richtigeMN==3 && richtigeEN==0) { // 
        return 16.3;
    }
    if(richtigeMN==2 && richtigeEN==2) { // 
        return 24.0;
    }
    if(richtigeMN==2 && richtigeEN==1) { // 
        return 9.1;
    }
    if(richtigeMN==1 && richtigeEN==2) { // 
        return 11.9;
    }
    return 0.0;
}

int main(int argc, char ** argv) {
    bool simulation_on = false;

    if(!(argc==1 || argc==4)) {
        std::cout<<"Usage: "<<argv[0]<<std::endl;
        std::cout<<"     for output 10 random numbers\n\n";
        std::cout<<"       "<<argv[0]<<" numbers 1-50 and 1-12 coma separated and rounds to play\n";
        std::cout<<"     example: "<<argv[0]<<" 3,12,34,41,44  7,8  1000\n";
        std::cout<<"       for simulatin a game\n\n";
        return 1;
    } else if(argc==4)
        simulation_on = true;

    if(!simulation_on) {
        simplePrint10Numbers();
        return 0;
    }

    const float cost_pergame = 2.0; // 2.0 Euro per Box
    int GAMES_MAX = atoi(argv[3]);

    std::vector<int> mainNumbers = convert2vectorInt(argv[1]);
    std::vector<int> euroNumbers = convert2vectorInt(argv[2]);;
    
    float sum = 0.0;
    float gewin = 0.0;
    float gezahlt = 0.0;

    for(int i=0; i<GAMES_MAX; i++) {
        std::vector<int> spielMainNumbers = generateNumbers(5, 1, 50);
        std::vector<int> spielEuroNumbers = generateNumbers(2, 1, 12);
        gezahlt -= cost_pergame;
        gewin += getGewin(mainNumbers, euroNumbers, spielMainNumbers, spielEuroNumbers);
    }
    sum = gewin + gezahlt;

    std::cout<<"Gespielt "<<GAMES_MAX<<" Runden\n";
    std::cout<<"Gezahlt: "<<gezahlt/1000<<" t€\n";
    std::cout<<"Gewin: "<<gewin/1000<<" t€\n";
    std::cout<<"SUME: "<<sum/1000<<" t€\n";
    std::cout<<"percent Gewin: "<<-gewin/gezahlt*100<<" %\n";

    return 0;
}