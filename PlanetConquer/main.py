import pygame
import random
import math

# Define constants
WIDTH = 800
HEIGHT = 600
FPS = 30
PLANET_RADIUS = 25

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
pygame.display.set_caption("PlanetConquer")
clock = pygame.time.Clock()

# Define game classes
class Planet(pygame.sprite.Sprite):
    def __init__(self, x, y, color, owner, production):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((PLANET_RADIUS*2, PLANET_RADIUS*2))
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.center = (x, y)
        self.owner = owner
        self.ships = 100
        self.production = production
        self.defense = self.ships
        self.color = color

    def update(self):
        self.ships += self.production
        if self.ships < 0:
            self.ships = 0
        self.defense = self.ships

    def attack(self, attacker):
        defense_power = self.defense + random.randint(0, 20)
        attack_power = attacker.ships - defense_power
        if attack_power > 0:
            self.owner = attacker.owner
            self.ships = attack_power
        else:
            self.ships = abs(attack_power)
        self.defense = self.ships

class Ship(pygame.sprite.Sprite):
    def __init__(self, start_planet, target_planet, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((10, 10))
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.start_planet = start_planet
        self.target_planet = target_planet
        self.color = color
        self.travel_time = math.sqrt((target_planet.rect.center[0]-start_planet.rect.center[0])**2 + (target_planet.rect.center[1]-start_planet.rect.center[1])**2) / 10
        self.start_time = pygame.time.get_ticks()

    def update(self):
        time_since_start = pygame.time.get_ticks() - self.start_time
        if time_since_start < self.travel_time * 1000:
            x = self.start_planet.rect.center[0] + (self.target_planet.rect.center[0]-self.start_planet.rect.center[0]) * (time_since_start / (self.travel_time * 1000))
            y = self.start_planet.rect.center[1] + (self.target_planet.rect.center[1]-self.start_planet.rect.center[1]) * (time_since_start / (self.travel_time * 1000))
            self.rect.center = (x, y)
        else:
            self.target_planet.ships += 1
            self.kill()

# Create game objects
all_sprites = pygame.sprite.Group()
planets = [Planet(100, 100, RED, "Enemy", random.randint(-5, 5)), Planet(700, 500, GREEN, "Player", random.randint(-5, 5)), Planet(400, 300, BLUE, "Neutral", random.randint(-5, 5))]
ships = pygame.sprite.Group()

# Game loop
running = True
while running:
    # Process events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                for planet in planets:
                    if planet.rect.collidepoint(event.pos):
                        print(f"Planet: {planet.owner} {planet.ships} ships {planet.defense} defense")
                    elif planet.owner == "Player":
                        if planet.rect.collidepoint(event.pos):
                            pass
                        elif planet.ships > 0:
                            target_planet = random.choice([p for p in planets if p.owner == "Enemy"])
                            ships.add(Ship(planet, target_planet, GREEN))
                            planet.ships -= 1

    # Update game
    for planet in planets:
        planet.update()
    for ship in ships:
        ship.update()
    for planet in planets:
        for other_planet in planets:
            if planet != other_planet and planet.owner == "Player" and other_planet.owner != "Player":
                distance = math.sqrt((other_planet.rect.center[0]-planet.rect.center[0])**2 + (other_planet.rect.center[1]-planet.rect.center[1])**2)
                if distance <= PLANET_RADIUS*2 and planet.ships > 0:
                    ships.add(Ship(planet, other_planet, GREEN))
                    planet.ships -= 1
                elif distance <= 300 and planet.ships > 0:
                    ships.add(Ship(planet, other_planet, GREEN))
                    planet.ships -= 1
                elif distance <= 500 and planet.ships > 10:
                    ships.add(Ship(planet, other_planet, GREEN))
                    planet.ships -= 10

    # Draw game objects
    screen.fill(BLACK)
    for planet in planets:
        pygame.draw.circle(screen, planet.color, planet.rect.center, PLANET_RADIUS)
        text_surface = pygame.font.Font(None, 20).render(f"{planet.ships}", True, WHITE)
        text_rect = text_surface.get_rect(center=planet.rect.center)
        screen.blit(text_surface, text_rect)
    for ship in ships:
        pygame.draw.line(screen, ship.color, ship.start_planet.rect.center, ship.target_planet.rect.center, 2)
        screen.blit(ship.image, ship.rect)
    pygame.display.flip()

    # Maintain constant frame rate
    clock.tick(FPS)

# Quit Pygame
pygame.quit()
