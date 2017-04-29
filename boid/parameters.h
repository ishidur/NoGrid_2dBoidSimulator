#pragma once
/*prameters*/

#define GRID_NO 10 //number of grid; boid can see around 1 grid
#define BOID_SIZE 0.5 //size of boid
#define BLOCK_SIZE 0.3 //size of block
#define BOID_SPEED 3.0 //initial boid speed
#define BOIDS_NO 30 //number of boids
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 20.0 //area boundary
#define VIEW_ANGLE 360.0 //boid view angle: degree
#define REPEL_WEIGHT 0.8 //repel force weight

#define OPTIMUM_DISTANCE 2.0*BOUNDARY/GRID_NO //optimum distance
#define CENTRIPETAL_WEIGHT 0.1 //centripetal  weight
#define ALIGNMENT_WEIGHT 0.2 //alignment weight
#define ACCEL 1.3 //accelaration
#define MAXSPEED 8.0 //boid: max speed
#define MINSPEED 2.0//boid: minimum speed