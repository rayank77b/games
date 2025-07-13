// === Gitter.cpp ===
#include "Gitter.h"
#include <ctime>
#include <iostream>


void Gitter::init(){
    clearGitter();
    clearGitterNew();

    std::srand(std::time(nullptr));
    std::cout<<std::rand()<<std::endl;

    for(int i=0; i<3000; i++) {
        int x = std::rand()%grid_count;
        int y = std::rand()%grid_count;
        gitter[x][y] = true;
    }
}

void Gitter::update(){
    clearGitterNew();
    // calculate new life
    for (int i=1; i<grid_count-1; i++) {
        for (int j=1; j<grid_count-1; j++) {
            int sum = calculateNeighbor(i, j);
            if(gitter[i][j]==false) { // cell is void
                if(sum==3)
                    gitter_new[i][j] = true; // new life
            } else {
                gitter_new[i][j] = true;  // live cell
                if(sum<2||sum>3) // alone or too many
                    gitter_new[i][j] = false;
            }
        }
    }
    // copy
    for (int i=0; i<grid_count; i++) {
        for (int j=0; j<grid_count; j++) {
            gitter[i][j] = gitter_new[i][j];
        }
    }
}

int Gitter::calculateNeighbor(int x, int y) {
    int sum = 0;

    for(int i=-1; i<2; i++) 
        for(int j=-1; j<2; j++) 
            if(gitter[x+i][y+j])
                sum++;
    
    if(gitter[x][y])// dont sum himself
        sum--;
    
    return sum;
}

void Gitter::clearGitter(){
    for (int i=0; i<grid_count; i++) {
        for (int j=0; j<grid_count; j++) {
            gitter[i][j] = false;
        }
    }
}

void Gitter::clearGitterNew(){
    for (int i=0; i<grid_count; i++) {
        for (int j=0; j<grid_count; j++) {
            gitter_new[i][j] = false;
        }
    }
}

void Gitter::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i=0; i<grid_count; i++) {
        for (int j=0; j<grid_count; j++) {
            if( gitter[i][j]==true) {
                SDL_Rect rect = { i * grid_width, j * grid_width, grid_width-1, grid_width-1 };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}