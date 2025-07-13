#!/usr/bin/python3.9

background_image_file = 'earth_moon.jpg'
rakete_image_file = 'ship.png'

import pygame
from pygame.locals import *
from sys import exit

pygame.init()

SCREEN_SIZE = (640, 480)
screen = pygame.display.set_mode(SCREEN_SIZE, FULLSCREEN, 32)
pygame.display.set_caption("Hello Wolrd")

background = pygame.image.load(background_image_file).convert()
rakete = pygame.image.load(rakete_image_file).convert_alpha()

x, y = 0, 0
move_x, move_y = 0, 0 


while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
        if event.type == KEYDOWN:
            if event.key == K_q:
                exit()
            if event.key == K_LEFT:
                move_x = -1
            if event.key == K_RIGHT:
                move_x = +1
            if event.key == K_UP:
                move_y = -1
            if event.key == K_DOWN:
                move_y = +1
        elif event.type == KEYUP:
            if event.key == K_LEFT:
                move_x = 0
            if event.key == K_RIGHT:
                move_x = 0
            if event.key == K_UP:
                move_y = 0
            if event.key == K_DOWN:
                move_y = 0
    x += move_x
    y += move_y

    screen.blit(background, (0,0))
    screen.blit(rakete, (x,y))

    pygame.display.update()