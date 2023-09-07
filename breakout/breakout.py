import pygame
import random
import math
from pygame.locals import *

# Define constants
WIDTH = 800
HEIGHT = 600
FPS = 30

# Define colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

class Square(pygame.sprite.Sprite):
    def __init__(self):
        super(Square, self).__init__()
        self.surf = pygame.Surface((100, 25))
        self.surf.fill(BLUE)
        self.rect = self.surf.get_rect()


# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Breakout")
clock = pygame.time.Clock()

square1 = Square()

gameOn = True

while gameOn:
    for event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.key == K_BACKSPACE:
                gameOn = False
        elif event.type == QUIT:
            gameOn = False

    screen.blit(square1.surf, (200, HEIGHT-25-10))
    pygame.display.flip()

# Quit Pygame
pygame.quit()
