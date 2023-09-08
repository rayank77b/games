import pygame
from pygame.locals import *

class Snake:
    def __init__(self, ss_x, ss_y):
        self.size = 10
        self.screen_size_x = int(ss_x/self.size)
        self.screen_size_y = int(ss_y/self.size)
        x = int(self.screen_size_x/2)
        y = int(self.screen_size_y/2)
        self.vx = +1
        self.vy = 0
        self.appel_cnt = 0
        self.body = []
        self.body.append((x, y))
        self.body.append((x-1, y))
        self.body.append((x-2, y))
    
    def set_v(self, vx, vy):
        self.vx = vx
        self.vy = vy
    
    def move(self):
        x, y = self.body[0]
        if self.appel_cnt > 0:
            self.appel_cnt -= 1
        else:
            self.body.pop()  # letztes abschneiden
        self.body.insert(0, (x+self.vx, y+self.vy))
    
    def eat_apel(self):
        self.appel_cnt = 3
    
    def collision(self):
        x, y = self.body[0]
        if x > self.screen_size_x  or x < 0:
            return True
        if y > self.screen_size_y  or y < 0:
            return True
        return False
    
    def event(self, event):
        if event.key == K_LEFT:
            self.set_v(-1,0)
        if event.key == K_RIGHT:
            self.set_v(1,0)
        if event.key == K_UP:
            self.set_v(0,-1)
        if event.key == K_DOWN:
            self.set_v(0,1)
    
    def draw(self, screen):
        for x, y in self.body:
            pygame.draw.rect(screen, (0,255,0), Rect((x*self.size, y*self.size), (self.size, self.size)))