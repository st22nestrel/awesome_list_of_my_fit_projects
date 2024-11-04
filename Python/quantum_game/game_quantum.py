import pygame
import time
from pygame.locals import (
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT
)
import random # we use randomness for the position and speed of enemies

high_score = 0
score = 0

class gameState():
    super_position = False
    not_switch = False


SCREEN_WIDTH = 800
SCREEN_HEIGHT = 400
SCREEN_HALF_HEIGHT = SCREEN_HEIGHT // 2

PLAYER_WIDTH = 75
PLAYER_HEIGHT = 25

pygame.init()
screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])

class Shell(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super(Shell, self).__init__()
        self.surf = pygame.Surface((10, 5)) # shells are smaller than enemies
        self.surf.fill((255, 0, 0)) # the color of shells
        self.rect = self.surf.get_rect(
            center=(
                x,
                y
            )
        )
        self.speed = 25

    def update(self):
        self.rect.move_ip(self.speed, 0)
        if self.rect.left > SCREEN_WIDTH: # remove any enemy moveing out side of the screen
            self.kill() # a nice method inherited from Sprite()

class Player(pygame.sprite.Sprite): # define this class before the infinite loop
    def __init__(self):
        super(Player, self).__init__() # execute the __init__ method of the parent (Sprite object)
        self.surf = pygame.Surface((PLAYER_WIDTH, PLAYER_HEIGHT)) # create a surface <- our photonic ship
        self.surf.fill((10, 255, 255)) # color of our photonic ship
        self.rect = self.surf.get_rect() # create a variable to access the surface as a rectangle
        self.shoot = False
        self.shells = pygame.sprite.Group() # keep all shots
    
    def update(self, pressed_keys): # we move the rectangular with (x,y)
        if gameState.super_position:
            return
        if self.shoot:
            new_shell = Shell(player.rect.right, player.rect.centery)
            self.shells.add(new_shell)
            self.shoot = False
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
        # Not is applied - lower screen
        if gameState.not_switch:
            if self.rect.top <= SCREEN_HALF_HEIGHT:
                self.rect.top = SCREEN_HALF_HEIGHT
            if self.rect.bottom >= SCREEN_HEIGHT:
                self.rect.bottom = SCREEN_HEIGHT
        # upper screen
        else:
            if self.rect.top <= 0:
                self.rect.top = 0
            if self.rect.bottom >= SCREEN_HALF_HEIGHT:
                self.rect.bottom = SCREEN_HALF_HEIGHT
            
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
        self.speed = random.randint(3, 10) # we assign a random speed - how many pixel to move to the left in each frame

    def update(self):
        if gameState.super_position:
            if pressed_keys[K_UP]:
                self.rect.move_ip(0, -5)
            if pressed_keys[K_DOWN]:
                self.rect.move_ip(0, 5)
        self.rect.move_ip(-self.speed, 0)

        if self.rect.right < 0: # remove any enemy moveing out side of the screen
            self.kill() # a nice method inherited from Sprite()

def display_msg(msg: str):
    my_font = pygame.font.SysFont('Comic Sans MS', 28) # create a font object
    # we create a text surface to blit on the screen
    text_surface = my_font.render(msg, False, (255,255,0),(0,0,0)) # message / anti-aliasing effect / text color / background color
    screen.blit(text_surface, (SCREEN_WIDTH/8, SCREEN_HEIGHT/2-text_surface.get_height()/2)) # blit the text on the screen with the specified position
    pygame.display.flip() # show everything since the last frame
    clock.tick(60) # set the FPS rate
    time.sleep(0.5)

player = Player() # define an instance -> our photonic ship
player2 = Player()

enemies = pygame.sprite.Group() # keep all enemies - the enemies will be added in the game infinite loop
all_sprites = pygame.sprite.Group() # keep all enemies and player(s)
all_sprites.add(player) # add the player here

clock = pygame.time.Clock() # define a clock object to set the FPS later inside the loop

CREATING_ENEMY_TIME_INTERVAL = 1250 # later we can set it to different values if we wish
ADDENEMY = pygame.USEREVENT + 1 # each event is associated with an integer
pygame.time.set_timer(ADDENEMY, CREATING_ENEMY_TIME_INTERVAL)

QUANTUM_EVENT_INTERVAL = 6000
QUANTUM_EVENT = pygame.USEREVENT
pygame.time.set_timer(QUANTUM_EVENT, QUANTUM_EVENT_INTERVAL)

there_is_message = False
running = True
playing = True
while running:
    if playing:
        for event in pygame.event.get(): # check all events one by one since the last frame 
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    player.shoot = True
                elif event.key == pygame.K_ESCAPE:
                    running = False # let's exit the game if the player press "ESC"
            if event.type == pygame.QUIT: # if the window is closed
                running = False
            elif event.type == ADDENEMY:  # we catch the new event here and then we will create a new enemy
                new_enemy = Enemy()
                enemies.add(new_enemy)  # add the new enemy
                all_sprites.add(new_enemy)  # add the new enemy
                score += 1
            elif event.type == QUANTUM_EVENT:
                rand = random.randint(0, 300)

                # NOT op
                if rand < 150:
                    if gameState.super_position:
                        continue #?
                    display_msg("NOT operation")
                    if gameState.not_switch:
                        player.rect.y = player.rect.y - SCREEN_HALF_HEIGHT
                        gameState.not_switch = False
                    else:
                        player.rect.y = player.rect.y + SCREEN_HALF_HEIGHT
                        gameState.not_switch = True
                # HADAMARD
                elif rand < 300:
                    display_msg("HADAMARD operation")
                    gameState.super_position = not gameState.super_position
                    if gameState.super_position:
                        player2.rect.x = player.rect.x
                        player2.rect.y = player.rect.y - SCREEN_HALF_HEIGHT if gameState.not_switch else player.rect.y + SCREEN_HALF_HEIGHT
    else:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_r and not playing:
                    score = 0
                    for enemy in enemies:
                        enemy.kill()
                    playing = True
                elif event.key == pygame.K_ESCAPE:
                    running = False # let's exit the game if the player press "ESC"
            
    screen.fill((0, 0, 0)) # the screen background color is set to black (Red=0,Green=0,Blue=0)
    # draw a line on the screen with specified color and starting and ending coordinates, from (x1,y1) to (x2,y2)
    pygame.draw.line(screen, (0, 255, 0), (0, SCREEN_HALF_HEIGHT), (SCREEN_WIDTH, SCREEN_HALF_HEIGHT))
    pressed_keys = pygame.key.get_pressed()

    if playing:
        player.update(pressed_keys)
        player.shells.update()
        enemies.update() # a nice property of sprite groups
        for entity in enemies:
            screen.blit(entity.surf, entity.rect)

        if gameState.super_position:
            p1_collides = pygame.sprite.spritecollideany(player, enemies)
            p2_collides = pygame.sprite.spritecollideany(player2, enemies)
            if p1_collides or p2_collides:
                display_msg("Measurment in progress")

                rand = random.randint(0,1)
                if (rand and p1_collides) or (not rand and p2_collides):
                    playing = False
                else:
                    display_msg("You were not hit by the enemy")
                    gameState.super_position = False
                    if p1_collides:
                        gameState.not_switch = not gameState.not_switch
                        player.rect.x = player2.rect.x
                        player.rect.y = player2.rect.y
        
        else:
            if pygame.sprite.spritecollideany(player, enemies): # check if "player" is hit by any entity in the "enemies" group
                playing = False


        screen.blit(player.surf, player.rect) # player is "transferred as a block" on the screen

        for shell in player.shells:
            screen.blit(shell.surf, shell.rect)
            killed_enemy = pygame.sprite.spritecollideany(shell, enemies)
            if killed_enemy:
                killed_enemy.kill()

        if gameState.super_position:
            screen.blit(player2.surf, player2.rect)

        # Display score and highscore
        my_font = pygame.font.SysFont('Comic Sans MS', 28) # create a font object
        # we create a text surface to blit on the screen
        score_text = my_font.render(f"Score: {score}", False, (255,255,0),(0,0,0)) # message / anti-aliasing effect / text color / background color
        high_score_text = my_font.render(f"High Score: {high_score if high_score > score else score}", False, (255,255,0),(0,0,0))
        screen.blit(score_text, (SCREEN_WIDTH - score_text.get_width(), 10)) # blit the text on the screen with the specified position
        screen.blit(high_score_text, (SCREEN_WIDTH - high_score_text.get_width(), 10+score_text.get_height())) # blit the text on the screen with the specified position
        pygame.display.flip() # show everything since the last frame
        clock.tick(60) # set the FPS rate
    
    else:
        font = pygame.font.SysFont('Comic Sans MS', 28) # create a font object
        text = font.render("Press 'R' to reload or 'ESC' to exit", False, (255, 100, 100))
        
        screen.blit(text, (SCREEN_WIDTH/2, SCREEN_HEIGHT/2))

        high_score_text = font.render(f"Your score: {score} High score: {high_score}", False, (255, 0, 0))
        screen.blit(high_score_text, (SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 28))
        if score > high_score or score == high_score:
            high_score = score
            congrats = font.render("New high score!", False, (255, 0, 0))
            screen.blit(congrats, (SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 56))

        pygame.display.flip() # show everything since the last frame
    
    ##    

if there_is_message: 
    time.sleep(2) # sleep for 2 seconds