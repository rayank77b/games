import pygame
import random

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

# Initialize Pygame
pygame.init()
pygame.mixer.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Real-time Strategy Game")
clock = pygame.time.Clock()

# Define game classes
class Structure(pygame.sprite.Sprite):
    def __init__(self, x, y, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((50, 50))
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)

class Unit(pygame.sprite.Sprite):
    def __init__(self, x, y, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((25, 25))
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)

class FogOfWar(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((50, 50))
        self.image.fill(BLACK)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.visible = False

    def update(self, visible):
        self.visible = visible
        if self.visible:
            self.image.fill(BLACK)
        else:
            self.image.fill(WHITE)

# Define game functions
def draw_text(text, font, color, x, y):
    text_surface = font.render(text, True, color)
    text_rect = text_surface.get_rect()
    text_rect.midtop = (x, y)
    screen.blit(text_surface, text_rect)

# Create game objects
all_sprites = pygame.sprite.Group()
structures = pygame.sprite.Group()
units = pygame.sprite.Group()
fog_of_war = pygame.sprite.Group()

# Create structures and units
for i in range(5):
    x = random.randint(100, WIDTH - 100)
    y = random.randint(100, HEIGHT - 100)
    color = random.choice([RED, GREEN, BLUE])
    structure = Structure(x, y, color)
    all_sprites.add(structure)
    structures.add(structure)

for i in range(10):
    x = random.randint(100, WIDTH - 100)
    y = random.randint(100, HEIGHT - 100)
    color = random.choice([RED, GREEN, BLUE])
    unit = Unit(x, y, color)
    all_sprites.add(unit)
    units.add(unit)

# Create fog of war
for x in range(0, WIDTH, 50):
    for y in range(0, HEIGHT, 50):
        fog_tile = FogOfWar(x, y)
        all_sprites.add(fog_tile)
        fog_of_war.add(fog_tile)

# Game loop
running = True
while running:
    # Process events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Update game
    mouse_pos = pygame.mouse.get_pos()
    for tile in fog_of_war:
        if tile.rect.collidepoint(mouse_pos):
            tile.update(True)
        else:
            tile.update(False)
