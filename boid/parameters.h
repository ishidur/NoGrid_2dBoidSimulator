#pragma once
/*prameters*/
/*
 *rule1: Alignment 
 *rule2: Cohesion
 *rule3: Separation-boid
 *rule4: Separation-block
 *rule5: Inertial
 */
#define GRID_NO 15 //number of grid; boid can find around 1 grid
#define WALL_SIZE 5.0 //size of wall
#define BOID_SIZE 10.0 //size of boid
#define BLOCK_SIZE 10.0 //size of block
#define MOUSE_SIZE 10.0 //size of mouse pointer
#define BOID_SPEED 3.0 //initial boid speed
#define BOIDS_NO 40 //initial number of boids
#define BLOCK_NO 10 //initial number of blocks
#define CIRCLE_SLICE 10 //circle slices
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 500.0 //area boundary
#define REPEL_WALL_WEIGHT 30.0 //repel force weight
#define MOUSE_ATTRACTION_FORCE 5.0 //mouse attraction weight
#define MOUSE_DISTRACTION_FORCE 2.0 //mouse distraction weight
#define ALPHA_1 0.5 //rule1 weight
#define ALPHA_2 1.1 //rule2 weight
#define ALPHA_3 2.0 //rule3 weight
#define ALPHA_4 5.0 //rule4 weight
#define ALPHA_5 2.0 //rule5 weight
#define R_1 50.0 //rule1 range
#define R_2 50.0 //rule2 range
#define R_3 30.0 //rule3 range
#define R_4 40.0 //rule4 range
#define THETA_1 240.0 //rule1 view angle
#define THETA_2 240.0 //rule2 view angle
#define THETA_3 240.0 //rule3 view angle
#define THETA_4 240.0 //rule4 view angle
#define BETA 40.0 //boid speed factor