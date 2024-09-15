#pragma once
// Maze Textures
extern unsigned int maze_tex;
extern unsigned int pill_tex;
extern unsigned int bigPill_tex;

// Pacman Textures
extern unsigned int pac_0_tex;
extern unsigned int pac_1_tex;
extern unsigned int pac_2_tex;
extern unsigned int dead_0_tex;
extern unsigned int dead_1_tex;
extern unsigned int dead_2_tex;
extern unsigned int dead_3_tex;
extern unsigned int dead_4_tex;
extern unsigned int dead_5_tex;
extern unsigned int dead_6_tex;
extern unsigned int dead_7_tex;
extern unsigned int dead_8_tex;
extern unsigned int dead_9_tex;
extern unsigned int dead_10_tex;

// Ghost Textures
extern unsigned int ghost_r_0_tex;
extern unsigned int ghost_r_1_tex;
extern unsigned int ghost_p_0_tex;
extern unsigned int ghost_p_1_tex;
extern unsigned int ghost_b_0_tex;
extern unsigned int ghost_b_1_tex;
extern unsigned int ghost_y_0_tex;
extern unsigned int ghost_y_1_tex;
extern unsigned int ghost_scared_0_tex;
extern unsigned int ghost_scared_1_tex;
extern unsigned int ghost_scared_2_tex;
extern unsigned int ghost_scared_3_tex;

// Eye Textures
extern unsigned int eye_up_tex;
extern unsigned int eye_right_tex;
extern unsigned int eye_down_tex;
extern unsigned int eye_left_tex;

// UI Textures
extern unsigned int num_0_tex;
extern unsigned int num_1_tex;
extern unsigned int num_2_tex;
extern unsigned int num_3_tex;
extern unsigned int num_4_tex;
extern unsigned int num_5_tex;
extern unsigned int num_6_tex;
extern unsigned int num_7_tex;
extern unsigned int num_8_tex;
extern unsigned int num_9_tex;
extern unsigned int ready_tex;
extern unsigned int gameover_tex;
extern unsigned int youwon_tex;
extern unsigned int pac_happy;
extern unsigned int life_tex;
extern unsigned int lives_tex;
extern unsigned int score_tex;
extern unsigned int high_tex;
extern unsigned int previous_tex;
extern unsigned int paused_tex;
extern unsigned int pausehint_tex;
extern unsigned int unpausehint_tex;
extern unsigned int quithint_tex;
extern unsigned int restarthint_tex;
// Fruit Textures
extern unsigned int cherry_tex;
extern unsigned int strawberry_tex;

void loadAndBindTextures();
void drawTex(unsigned int texture, int length, int height, float angle);