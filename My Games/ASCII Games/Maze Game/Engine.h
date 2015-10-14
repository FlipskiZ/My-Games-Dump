#ifndef ENGINE_H
#define ENGINE_H

#define mapHeight 100
#define mapWidth 200

bool isPassable(int y, int x);

extern int mapArray[mapHeight][mapWidth];

extern bool generating;

extern int height;
extern int width;

extern int side;
#endif // ENGINE_H
