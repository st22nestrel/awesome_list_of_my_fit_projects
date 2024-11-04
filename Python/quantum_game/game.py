import pygame
import time
from pygame.locals import (
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
)

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 400

PLAYER_WIDTH = 75
PLAYER_HEIGHT = 25

pygame.init()
screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])

class Player(pygame.sprite.Sprite): # define this class before the infinite loop
    def __init__(self):
        super(Player, self).__init__() # execute the __init__ method of the parent (Sprite object)
        self.surf = pygame.Surface((PLAYER_WIDTH, PLAYER_HEIGHT)) # create a surface <- our photonic ship
        self.surf.fill((10, 255, 255)) # color of our photonic ship
        self.rect = self.surf.get_rect() # create a variable to access the surface as a rectangle
    
    def update(self, pressed_keys): # we move the rectangular with (x,y)
        if pressed_keys[K_UP]:
            self.rect.move_ip(0, -5)
        if pressed_keys[K_DOWN]:
            self.rect.move_ip(0, 5)
        if pressed_keys[K_LEFT]:
            self.rect.move_ip(-5, 0)
        if pressed_keys[K_RIGHT]:
            self.rect.move_ip(5, 0)
        # Keep player on the screen
        if self.rect.left < 0:
            self.rect.left = 0
        if self.rect.right > SCREEN_WIDTH:
            self.rect.right = SCREEN_WIDTH
        if self.rect.top <= 0:
            self.rect.top = 0
        if self.rect.bottom >= SCREEN_HEIGHT:
            self.rect.bottom = SCREEN_HEIGHT

import random # we use randomness for the position and speed of enemies
class Enemy(pygame.sprite.Sprite):
    def __init__(self):
        super(Enemy, self).__init__()
        self.surf = pygame.Surface((20, 10)) # Enemies are smaller than our photonic ship
        self.surf.fill((255, 255, 255)) # the color of enemies - would you like to try different colors here?
        self.rect = self.surf.get_rect( # their positions are random but still they should appear on the right side
            center=(
                random.randint(SCREEN_WIDTH + 20, SCREEN_WIDTH + 100), # position of x
                random.randint(0, SCREEN_HEIGHT), # position of y
            )
        )
        self.speed = random.randint(5, 20) # we assign a random speed - how many pixel to move to the left in each frame

    def update(self):
        self.rect.move_ip(-self.speed, 0)
        if self.rect.right < 0: # remove any enemy moveing out side of the screen
            self.kill() # a nice method inherited from Sprite()

player = Player() # define an instance -> our photonic ship

enemies = pygame.sprite.Group() # keep all enemies - the enemies will be added in the game infinite loop
all_sprites = pygame.sprite.Group() # keep all enemies and player(s)
all_sprites.add(player) # add the player here

clock = pygame.time.Clock() # define a clock object to set the FPS later inside the loop

CREATING_ENEMY_TIME_INTERVAL = 250 # later we can set it to different values if we wish
ADDENEMY = pygame.USEREVENT + 1 # each event is associated with an integer
pygame.time.set_timer(ADDENEMY, CREATING_ENEMY_TIME_INTERVAL)

there_is_message = False
running = True
while running:
    for event in pygame.event.get(): # check all events one by one since the last frame 
        if event.type == pygame.QUIT: # if the window is closed
            running = False
        elif event.type == ADDENEMY:  # we catch the new event here and then we will create a new enemy
            new_enemy = Enemy()
            enemies.add(new_enemy)  # add the new enemy
            all_sprites.add(new_enemy)  # add the new enemy
    screen.fill((0, 0, 0)) # the screen background color is set to black (Red=0,Green=0,Blue=0)
    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_ESCAPE]:
        running = False # let's exit the game if the player press "ESC"
    player.update(pressed_keys)
    enemies.update() # a nice property of sprite groups
    for entity in all_sprites:
        screen.blit(entity.surf, entity.rect)

    if pygame.sprite.spritecollideany(player, enemies): # check if "player" is hit by any entity in the "enemies" group
        # if so: then remove the player and stop the infinite loop
        player.kill()
        running = False
        pygame.display.flip() # show everything since the last frame
        clock.tick(30) # set the FPS rate
        
        my_font = pygame.font.SysFont('Comic Sans MS', 28) # create a font object
        # we create a text surface to blit on the screen
        text_surface = my_font.render("Game Over :( ", False, (255,255,0),(0,0,0)) # message / anti-aliasing effect / text color / background color
        screen.blit(text_surface, (SCREEN_WIDTH/8, SCREEN_HEIGHT/2-text_surface.get_height()/2)) # blit the text on the screen with the specified position
        there_is_message = True

    #screen.blit(player.surf, player.rect) # player is "transferred as a block" on the screen 
    pygame.display.flip() # show everything since the last frame
    clock.tick(30) # set the FPS rate

if there_is_message: 
    time.sleep(2) # sleep for 2 seconds