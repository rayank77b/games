import pygame
import random
import math
from pygame.locals import *

# Define constants
WIDTH = 800
HEIGHT = 600
FPS = 30
BRICK_W = 100   # width
BRICK_H = 25    # height
BALL_R = 10     # radius

# Define colors
WHITE =  (255, 255, 255)
BLACK =  (  0,   0,   0)
RED =    (255,   0,   0)
GREEN =  (  0, 255,   0)
BLUE =   (  0,   0, 255)
YELLOW = (  0, 255, 255) 

class Background(pygame.sprite.Sprite):
    def __init__(self):
        super(Background, self).__init__()
        self.surf = pygame.Surface((WIDTH, HEIGHT))
        self.surf.fill(BLACK)
        self.rect = self.surf.get_rect()

class Ball(pygame.sprite.Sprite):
    def __init__(self):
        super(Ball, self).__init__()
        self.surf = pygame.Surface((BALL_R*2, BALL_R*2))
        self.surf.fill(BLACK)
        pygame.draw.circle(self.surf, RED, (BALL_R, BALL_R), BALL_R)
        self.rect = self.surf.get_rect()

class Brick(pygame.sprite.Sprite):
    def __init__(self, hits=1):
        super(Brick, self).__init__()
        self.surf = pygame.Surface((BRICK_W, BRICK_H))
        self.hits = hits
        self.surf.fill(self.getcolor())
        self.rect = self.surf.get_rect()
    
    def getcolor(self):
        if(self.hits == 3):
            return RED
        elif(self.hits == 2):
            return YELLOW
        else:
            return GREEN

class PlayerBrick(pygame.sprite.Sprite):
    def __init__(self):
        super(PlayerBrick, self).__init__()
        self.surf = pygame.Surface((BRICK_W, BRICK_H))
        self.surf.fill(BLUE)
        self.rect = self.surf.get_rect()

            
# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Breakout")
clock = pygame.time.Clock()

background = Background()
ball   = Ball()
player = PlayerBrick()
brick1 = Brick(3)
brick2 = Brick(2)
brick3 = Brick(1)

gameOn = True
X_POS = 200
B_X   = 400
B_Y   = 300
V_X   = 1.2
V_Y   = -0.3

while gameOn:
    # get key event
    for event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.key == K_RIGHT:
                X_POS = X_POS + 10
            elif event.key == K_LEFT:
                X_POS = X_POS - 10
            elif event.key == K_q:
                gameOn = False
        elif event.type == QUIT:
            gameOn = False
    # calculate ball collision with wall
    if (B_X >= WIDTH) or (B_X <= 0):
        V_X *= -1
    if (B_Y >= HEIGHT) or (B_Y <= 0):
        V_Y *= -1
    # calculate position of ball
    dt = clock.tick(30)
    B_X = B_X + V_X*dt
    B_Y = B_Y + V_Y*dt
    # draw anything
    screen.blit(background.surf, (0, 0))
    screen.blit(ball.surf, (B_X, B_Y))
    screen.blit(player.surf, (X_POS, HEIGHT-BRICK_H-10))
    screen.blit(brick1.surf, (10, 10))
    screen.blit(brick2.surf, (200, 50))
    screen.blit(brick3.surf, (350, 100))
    pygame.display.flip()

# Quit Pygame
pygame.quit()
