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
X_POS = 200     # start position of player

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
    def __init__(self, vx, vy):
        super(Ball, self).__init__()
        self.r = BALL_R
        self.x = WIDTH/2
        self.y = HEIGHT/2
        self.vx = vx
        self.vy = vy
        self.pos = (self.x, self.y)
        self.dt = clock.tick(FPS)
        self.surf = pygame.Surface((self.r*2, self.r*2))
        self.surf.fill(BLACK)
        pygame.draw.circle(self.surf, RED, (self.r, self.r), self.r)
        self.rect = self.surf.get_rect()
    
    def hitwall(self):
        if (self.x >= WIDTH-self.r*2) or (self.x <= 0):
            self.vx *= -1
        if (self.y >= HEIGHT-self.r*2) or (self.y <= 0):
            self.vy *= -1
    
    def hitplayer(self, p):
        if (self.y+self.r*2) >= p.y :
            if (self.x >= p.x) and ((self.x+self.r*2) <= (p.x + p.w)) :
                self.vy *= -1
    
    def move(self):
        self.x += self.vx*self.dt
        self.y += self.vy*self.dt
        self.pos = (self.x, self.y)
    
    def drawme(self, screen):
        screen.blit(self.surf, self.pos)
        
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
            
    def drawme(self, screen):
        screen.blit(self.surf, self.pos)

class PlayerBrick(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(PlayerBrick, self).__init__()
        self.w = BRICK_W
        self.h = BRICK_H
        self.x = x
        self.y = y
        self.pos = (x,y)
        self.keypressed = False
        self.vx = 0
        self.surf = pygame.Surface((self.w, self.h))
        self.surf.fill(BLUE)
        self.rect = self.surf.get_rect()
    
    def keyevent(self, et, ek):
        if et == KEYDOWN:
            self.keypressed = True
            if ek == K_RIGHT:
                self.vx = 10 
            elif ek == K_LEFT:
                self.vx = -10
            else:
                self.vx = 0       # falls andere taste
        else:
            self.keypressed = False
            self.key = 0

    def move(self):  
        if self.keypressed:
            if self.x < 0:
                self.x = 0
            elif self.x > WIDTH-self.w:
                self.x = WIDTH-self.w
            else:
                self.vx *= 1.05      # longer hold, faster
                self.x += self.vx
                self.pos = (self.x, self.y)
    
    def drawme(self, screen):
        screen.blit(self.surf, self.pos)
            
# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Breakout")
clock = pygame.time.Clock()

background = Background()
ball   = Ball(0.1, 0.3)
player = PlayerBrick(X_POS, HEIGHT-BRICK_H-10)
brick1 = Brick(3)
brick2 = Brick(2)
brick3 = Brick(1)

gameOn = True

while gameOn:
    # get key event
    for event in pygame.event.get():
        if event.type == KEYDOWN or event.type == KEYUP:
            player.keyevent(event.type, event.key)
        if event.type == KEYDOWN:
            if event.key == K_q:
                gameOn = False
        elif event.type == QUIT:
            gameOn = False
    
    player.move()
    # get ball collision with wall
    ball.hitwall()
    # move ball
    ball.hitplayer(player)
    ball.dt = clock.tick(FPS)
    ball.move()
    
    # draw anything
    screen.blit(background.surf, (0, 0))
    ball.drawme(screen)
    player.drawme(screen)
    screen.blit(brick1.surf, (10, 10))
    screen.blit(brick2.surf, (200, 50))
    screen.blit(brick3.surf, (350, 100))
    pygame.display.flip()

# Quit Pygame
pygame.quit()
