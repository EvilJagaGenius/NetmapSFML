# I need a quick script for creating dummy layer bkgs for Netmap.
# Edit as necessary.

import pygame, sys
from pygame import *
pygame.init()

WX = 100
WY = 100
WINDOW = pygame.display.set_mode((WX, WY), 0, 32)

layerX = 2000
layerY = 576
color = (0,0,255)

surface = pygame.Surface((layerX, layerY), SRCALPHA)

for i in range(layerX):
    for j in range(layerY):
        if (i % 100 == 0) or (i == layerX-1) or (j == 0) or (j == layerY-1):
            # Draw the pixel
            surface.set_at((i, j), color)

pygame.image.save(surface, "Layer2.png")
pygame.quit()
sys.exit()  
