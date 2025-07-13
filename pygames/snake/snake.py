import pygame
from pygame.locals import *
from sys import exit
import Snake

SCREEN_SIZE = (640, 480)
CLOCK_TICKS = 50

pygame.init()
snake = Snake.Snake(SCREEN_SIZE[0], SCREEN_SIZE[1])

screen = pygame.display.set_mode(SCREEN_SIZE, 0, 32)

clock = pygame.time.Clock()
clock.tick(CLOCK_TICKS)
time_counter = 0

game_running = True
while game_running:
    time_counter += 1
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
    if event.type == KEYDOWN:
            if event.key == K_q:
                exit()
            snake.event(event)
    clock.tick(CLOCK_TICKS)
    if (time_counter>10):
        time_counter = 0
        snake.move()
        screen.lock()
        pygame.draw.rect(screen, (0,0,0), Rect((0,0), SCREEN_SIZE))
        snake.draw(screen)
        screen.unlock()

        pygame.display.update()

        game_running = not snake.collision()

print("ENDE")