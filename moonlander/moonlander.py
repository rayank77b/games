#!/usr/bin/python3

background_image_file = 'earth_moon.jpg'
sprite_image_file = 'ship.png'

import pygame
from pygame.locals import *
from sys import exit

pygame.init()

SCREEN_SIZE = (640, 480)
screen = pygame.display.set_mode(SCREEN_SIZE, 0, 32)
pygame.display.set_caption("Hello Wolrd")

background = pygame.image.load(background_image_file).convert()
sprite = pygame.image.load(sprite_image_file).convert_alpha()

clock = pygame.time.Clock()

x = 0
speed = 250

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()

    screen.blit(background, (0,0))
    screen.blit(sprite, (x,100))

    time_passed = clock.tick()
    time_passed_seconds = time_passed/1000.0
    distance_moved = time_passed_seconds*speed

    x += distance_moved

    if x>640:
        x-=640

    pygame.display.update()
