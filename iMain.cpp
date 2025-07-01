#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "iGraphics.h"
#include "iSound.h"

bool init = false;
float ratio = 0.8; //zoomed 0.8, full 1
int scene = 0;
int sub_scene = 0;
int prev_sub_scene = 0;
double tri_x[3];
double tri_y[3];
int x_position = -1920, y_position = -1080;
char x_pos[9], y_pos[9];
//x1,x2,y1,y2
int collision [26][4] = {{-1690,-1890,-1180,-1000},{-1950,-2140,-1180,-1000},{-1710,-2140,-1050,-760},{-2750,-3070,-1720,-1430},{-3490,-3920,-2220,-1930},{-4160,-4530,-1890,-1500},{-3970,-4400,-1260,-900},{-2810,-3520,-840,-90},{-3900,-4500,-40,260},{-1270,-2540,-2470,-2020},{630,20,-2410,-2080},{-270,-640,-1680,-1290},{-650,-1070,-1640,-1350},{-700,-1020,-720,-420},{-60,-490,-220,60},{640,260,-170,220},{690,170,-520,-350},{340,170,-700,-350},{750,300,-800,-510},{-1590,-1760,-70,270},{-1750,-2180,10,300},{-2170,-2340,-70,270},{990,640,-2530,420},{780,-4620,-2420,-2370},{760,-4630,230,570},{-4490,-4830,-2510,400}};
int interaction[10][4] = {{-1890,-1950,-1080,-1050},{-3540,-3870,-1930,-1920},{570,250,-2080,-2070},{640,360,-810,-800},{-4020,-4350,-900,-890},{-700,-1020,-1650,-1640},{-4140,-4430,-50,-40},{-1660,-2120,-2020,-2010},{-1800,-2130,0,10},{-110,-440,-230,-220}};
int move_speed = 10;
int counter = 0;
int current_ineteraction = -1;
bool card_toggle = false;
bool running = false;
int direction = 0; //0 up,1 left, 2 down, 3 right
int last_key_press = 0;
int hero_count = 0;

Image cover,menu,newGame,leaderboard,instruction, world, bh, cf, dl, e,lb, ltl, dyc, gm, gpc;
Image hero, hero1, hero2, hero3, hero4, hero5, hero6, hero7, hero8, hero9, hero10, hero11, hero12;
Image pass, fail, dead, valve1, valve2, dart1, dart2, bulb;
Image marked,cracked,one,two,three,four,five,six,zero;
FILE *score_ptr, *save_ptr;
int menu_bgm, world_bgm;
bool menu_bgm_status = true;
bool world_bgm_status = false;

//Level timer
int level_timer_list[6] = {1000,1000,3000,1000,1000,1000};
int score_list[7] = {500, 2000, 1000, 500, 2000, 1500, 1000};

//save/load info
char name[31] = "";
int name_cursor = 0;
char stringRead[100];
bool save = false;
int current_level = 1;
int level_timer = 0;
bool bonus_one = false;
bool bonus_two = false;
int score = 0;

//Universal Variable
int focus = 0;
int prev_focus = 0;
int sub_focus = 0;
bool win = false;
int win_counter = 300;

//Burning House (Scene 07 : Level 01)
int valve_order[5] = {3,2,5,4,1};
int current_valve = 0;
int rotate_count = 0;
int sub_rotate_count = 0;

//Cracked Floor (Scene 08 : Level 02)
bool tool = false; //FALSE marker TRUE step
int floor_map[11][12] = {
    -1,2,1,2,3,2,1,0,0,1,1,1,
    -1,3,2,-1,-1,-1,4,3,2,3,-1,2,
    -1,4,3,-1,5,-1,-1,-1,-1,6,-1,3,
    -1,-1,3,1,2,3,6,-1,-1,-1,-1,2,
    4,-1,3,0,0,1,-1,-1,-1,-1,6,3,
    2,-1,3,2,1,2,2,5,-1,-1,-1,-1,
    1,2,-1,3,-1,2,0,2,-1,-1,5,3,
    0,2,3,6,-1,4,1,1,2,2,3,-1,
    2,3,-1,-1,-1,-1,3,3,2,1,2,-1,
    -1,-1,3,3,3,3,-1,-1,-1,1,1,1,
    2,2,1,0,0,1,3,-1,3,1,0,0
}; //-1 crack, 0-6 number
int tool_map[11][12] = {
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,2,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
}; //0 untouched, 1, marked, 2 stepped
int step_count = 0;
int marker_count = 0;
int crack_count = 0;
bool failed = false;

//Door Locked Variables (Scene 09 : Level 03)
Image p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12;
int pieces[12] = {6,9,2,7,4,11,3,12,1,8,10,5};

//Let There be Light (Scene 12 : Level 06)
int bulb_pos[7][2] = {
    -1,-1,
    -2,-2,
    -3,-3,
    -4,-4,
    -5,-5,
    -6,-6,
    -7,-7
};
int base_map[11][12] = {
    1,0,1,0,1,0,1,1,1,0,1,1,
    0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,0,1,0,1,1,1,0,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,0,0,0,0,0,0,0,1,0,
    1,0,1,0,1,0,1,1,0,0,1,0,
    0,0,0,0,0,0,0,1,0,0,1,0,
    1,0,1,1,0,0,0,0,0,0,0,0,
    1,0,1,0,1,1,1,1,0,1,1,0,
    1,1,0,0,0,0,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,0,0,0,0
};
int bulb_idx = -1;
int last_bulb_idx = -1;
bool swapValid = true;
int bulb_i = 0;
int light_tile = 0;


//Do You Copy Variables (Scene 13 : Level 07)
int radio_knob1 = 0;
int radio_knob2 = 0;
int radio_button[6] = {0,0,0,0,0,0};
double cursor_pos = 0;

//Gambler Merchant Variables (Scene 14 : Level B1)
int aim_x = 0;
int aim_x_dir = 1;
int aim_y = 346;
int aim_y_dir = 1;
int dart_score = 0;
char dart_score_s[3];
int r, point;
int deg;
int dart_point[20] = {6,13,4,18,1,20,5,12,9,14,11,8,16,7,19,3,17,2,15,10};
int aim_move_speed = 4;
int dart_counter = 200;
int gamble_move_x[20] = {2,-164,-64,256,172,2,22,-22,148,230,16,2,0,82,-272,-44,206,156,2,144};
int gamble_move_y[20] = {0,-10,148,234,216,0,262,-158,-88,144,-10,0,278,-268,54,-214,60,-118,0,166};
int dart_random;
int merchant_score = 0;
int this_dart = 0;

void loadImages()
{
    iLoadImage(&cover, "assets/images/cover.png");
    iScaleImage(&cover, ratio);
    iLoadImage(&menu, "assets/images/main_menu.png");
    iScaleImage(&menu, ratio);
    iLoadImage(&newGame, "assets/images/new_game.png");
    iScaleImage(&newGame, ratio);
    iLoadImage(&leaderboard, "assets/images/leaderboard.png");
    iScaleImage(&leaderboard, ratio);
    iLoadImage(&instruction, "assets/images/instruction.png");
    iScaleImage(&instruction, ratio);
    iLoadImage(&world, "assets/images/world.png");
    iScaleImage(&world, 1/0.8*ratio);
    //Hero
    iLoadImage(&hero1, "assets/images/effects/hero1.png");
    iScaleImage(&hero1, ratio);
    iLoadImage(&hero2, "assets/images/effects/hero2.png");
    iScaleImage(&hero2, ratio);
    iLoadImage(&hero3, "assets/images/effects/hero3.png");
    iScaleImage(&hero3, ratio);
    iLoadImage(&hero4, "assets/images/effects/hero4.png");
    iScaleImage(&hero4, ratio);
    iLoadImage(&hero5, "assets/images/effects/hero5.png");
    iScaleImage(&hero5, ratio);
    iLoadImage(&hero6, "assets/images/effects/hero6.png");
    iScaleImage(&hero6, ratio);
    iLoadImage(&hero7, "assets/images/effects/hero7.png");
    iScaleImage(&hero7, ratio);
    iLoadImage(&hero8, "assets/images/effects/hero8.png");
    iScaleImage(&hero8, ratio);
    iLoadImage(&hero9, "assets/images/effects/hero9.png");
    iScaleImage(&hero9, ratio);
    iLoadImage(&hero10, "assets/images/effects/hero10.png");
    iScaleImage(&hero10, ratio);
    iLoadImage(&hero11, "assets/images/effects/hero11.png");
    iScaleImage(&hero11, ratio);
    iLoadImage(&hero12, "assets/images/effects/hero12.png");
    iScaleImage(&hero12, ratio);
    //scene
    iLoadImage(&bh, "assets/images/bh.png");
    iScaleImage(&bh, ratio);
    iLoadImage(&cf, "assets/images/cf.png");
    iScaleImage(&cf, ratio);
    iLoadImage(&dl, "assets/images/dl.png");
    iScaleImage(&dl, ratio);
    iLoadImage(&e, "assets/images/e.png");
    iScaleImage(&e, ratio);
    iLoadImage(&lb, "assets/images/lb.png");
    iScaleImage(&lb, ratio);
    iLoadImage(&ltl, "assets/images/ltl.png");
    iScaleImage(&ltl, ratio);
    iLoadImage(&dyc, "assets/images/dyc.png");
    iScaleImage(&dyc, ratio);
    iLoadImage(&gm, "assets/images/gm.png");
    iScaleImage(&gm, ratio);
    iLoadImage(&gpc, "assets/images/gpc.png");
    iScaleImage(&gpc, ratio);
    //Door Locked Images (Scene 09 : Level 03)
    iLoadImage(&p1, "assets/images/effects/p1.png");
    iScaleImage(&p1, ratio);
    iLoadImage(&p2, "assets/images/effects/p2.png");
    iScaleImage(&p2, ratio);
    iLoadImage(&p3, "assets/images/effects/p3.png");
    iScaleImage(&p3, ratio);
    iLoadImage(&p4, "assets/images/effects/p4.png");
    iScaleImage(&p4, ratio);
    iLoadImage(&p5, "assets/images/effects/p5.png");
    iScaleImage(&p5, ratio);
    iLoadImage(&p6, "assets/images/effects/p6.png");
    iScaleImage(&p6, ratio);
    iLoadImage(&p7, "assets/images/effects/p7.png");
    iScaleImage(&p7, ratio);
    iLoadImage(&p8, "assets/images/effects/p8.png");
    iScaleImage(&p8, ratio);
    iLoadImage(&p9, "assets/images/effects/p9.png");
    iScaleImage(&p9, ratio);
    iLoadImage(&p10, "assets/images/effects/p10.png");
    iScaleImage(&p10, ratio);
    iLoadImage(&p11, "assets/images/effects/p11.png");
    iScaleImage(&p11, ratio);
    iLoadImage(&p12, "assets/images/effects/p12.png");
    iScaleImage(&p12, ratio);
    //Effects
    iLoadImage(&pass, "assets/images/effects/pass.png");
    iScaleImage(&pass, ratio);
    iLoadImage(&fail, "assets/images/effects/fail.png");
    iScaleImage(&fail, ratio);
    iLoadImage(&dead, "assets/images/effects/dead.png");
    iScaleImage(&dead, ratio);
    iLoadImage(&valve1, "assets/images/effects/valve1.png");
    iScaleImage(&valve1, ratio);
    iLoadImage(&valve2, "assets/images/effects/valve2.png");
    iScaleImage(&valve2, ratio);
    iLoadImage(&dart1, "assets/images/effects/dart1.png");
    iScaleImage(&dart1, ratio);
    iLoadImage(&dart2, "assets/images/effects/dart2.png");
    iScaleImage(&dart2, ratio);
    iLoadImage(&marked, "assets/images/effects/marked.png");
    iScaleImage(&marked, ratio);
    iLoadImage(&cracked, "assets/images/effects/cracked.png");
    iScaleImage(&cracked, ratio);
    iLoadImage(&one, "assets/images/effects/1.png");
    iScaleImage(&one, ratio);
    iLoadImage(&two, "assets/images/effects/2.png");
    iScaleImage(&two, ratio);
    iLoadImage(&three, "assets/images/effects/3.png");
    iScaleImage(&three, ratio);
    iLoadImage(&four, "assets/images/effects/4.png");
    iScaleImage(&four, ratio);
    iLoadImage(&five, "assets/images/effects/5.png");
    iScaleImage(&five, ratio);
    iLoadImage(&six, "assets/images/effects/6.png");
    iScaleImage(&six, ratio);
    iLoadImage(&zero, "assets/images/effects/0.png");
    iScaleImage(&zero, ratio);
    iLoadImage(&bulb, "assets/images/effects/bulb.png");
    iScaleImage(&bulb, ratio);
}

void saveGame(void)
{
    save_ptr = fopen("saves/save.txt","w");
    fprintf(save_ptr, "YES\n");
    fprintf(save_ptr, "%s\n", name);
    fprintf(save_ptr, "%d\n", current_level);
    fprintf(save_ptr, "%d\n", level_timer);
    if(bonus_one == false) fprintf(save_ptr, "FALSE\n");
    else fprintf(save_ptr, "TRUE\n");
    if(bonus_two == false) fprintf(save_ptr, "FALSE\n");
    else fprintf(save_ptr, "TRUE\n");
    fprintf(save_ptr, "%d\n",score);
}


void newGameInit(void)
{
    printf("New Game");
    save = false;
    current_level = 1;
    level_timer = level_timer_list[0];
    bonus_one = false;
    bonus_two = false;
    score = 0;
    saveGame();
    scene = 5;
    sub_scene = 0;
}

void getPosition(void)
{
    int temp_x = abs(x_position), temp_y = abs(y_position);
    x_pos[0] = 'X', x_pos[1] = ':', x_pos[2] = ' ';
    if(x_position<0) x_pos[3] = '-';
    else x_pos[3] = ' ';
    x_pos[4] = (temp_x/1000)+'0';
    x_pos[5] = (temp_x/100)%10+'0';
    x_pos[6] = (temp_x/10)%10+'0';
    x_pos[7] = (temp_x)%10+'0';
    y_pos[0] = 'y', y_pos[1] = ':', y_pos[2] = ' ';
    if(y_position<0) y_pos[3] = '-';
    else y_pos[3] = ' ';
    y_pos[4] = (temp_y/1000)+'0';
    y_pos[5] = (temp_y/100)%10+'0';
    y_pos[6] = (temp_y/10)%10+'0';
    y_pos[7] = (temp_y)%10+'0';

}

bool checkMove(int pos_x, int pos_y)
{
    //pos_x = pos_x/0.8*ratio;
    //pos_y = pos_y/0.8*ratio;
    for(int c=0; c<26; c++)
    {
        if(pos_x > collision[c][1] && pos_x < collision[c][0] && pos_y > collision[c][2] && pos_y < collision[c][3])
            return false;
    }

    return true;
}

void interactionCheck(void)
{
    current_ineteraction = -1;
    for(int c=0; c<10; c++)
    {
        if(x_position >= interaction[c][1] && x_position <= interaction[c][0] && y_position >= interaction[c][2] && y_position <= interaction[c][3])
        {
            current_ineteraction = c;
            break;
        }
    }
    //printf("\n%d", current_ineteraction);
}

void heroMove(double x, double y)
{
    //running
    if(last_key_press != 0) last_key_press--;
    if( last_key_press != 0)
    {
        (hero_count == 15) ? hero_count = 0 : hero_count++;
        if(direction == 0)
        {
            if(hero_count <= 5) hero = hero1;
            else if(hero_count <= 10) hero = hero2;
            else if(hero_count <= 15) hero = hero3;
        }
        else if(direction == 1)
        {
            if(hero_count <= 5) hero = hero4;
            else if(hero_count <= 10) hero = hero5;
            else if(hero_count <= 15) hero = hero6;
        }
        else if(direction == 2)
        {
            if(hero_count <= 5) hero = hero7;
            else if(hero_count <= 10) hero = hero8;
            else if(hero_count <= 15) hero = hero9;
        }
        else if(direction == 3)
        {
            if(hero_count <= 5) hero = hero10;
            else if(hero_count <= 10) hero = hero11;
            else if(hero_count <= 15) hero = hero12;
        }
    }
    else
    {
        if(direction == 0) hero = hero1;
        else if(direction == 1) hero = hero4;
        else if(direction == 2) hero = hero7;
        else if(direction == 3) hero = hero10;
    }

    //if(direction == 2) iMirrorImage(&hero, VERTICAL);
    //else if(direction == 3) iMirrorImage(&hero, HORIZONTAL);

    iShowLoadedImage(x,y,&hero);
}

//Door Locked Functions (Scene 09 : Level 03)
void drawPiece(int img_id, int x, int y)
{
    switch(img_id)
    {
    case 1:
        iShowLoadedImage(x,y,&p1);
        break;
    case 2:
        iShowLoadedImage(x,y,&p2);
        break;
    case 3:
        iShowLoadedImage(x,y,&p3);
        break;
    case 4:
        iShowLoadedImage(x,y,&p4);
        break;
    case 5:
        iShowLoadedImage(x,y,&p5);
        break;
    case 6:
        iShowLoadedImage(x,y,&p6);
        break;
    case 7:
        iShowLoadedImage(x,y,&p7);
        break;
    case 8:
        iShowLoadedImage(x,y,&p8);
        break;
    case 9:
        iShowLoadedImage(x,y,&p9);
        break;
    case 10:
        iShowLoadedImage(x,y,&p10);
        break;
    case 11:
        iShowLoadedImage(x,y,&p11);
        break;
    case 12:
        iShowLoadedImage(x,y,&p12);
        break;
    }
}

void pieceFocus(void)
{
    switch(sub_focus)
    {
    case 1:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(801*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 2:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1074*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 3:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1345*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 4:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1616*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 5:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(801*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 6:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1074*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 7:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1345*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 8:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1616*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 9:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(801*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 10:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1074*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 11:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1345*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 12:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,0.4);
        iRectangle(1616*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    }

    switch(focus)
    {
    case 1:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(801*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 2:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1074*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 3:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1345*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 4:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1616*ratio, 683*ratio, 260*ratio, 262*ratio);
        break;
    case 5:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(801*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 6:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1074*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 7:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1345*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 8:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1616*ratio, 409*ratio, 260*ratio, 262*ratio);
        break;
    case 9:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(801*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 10:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1074*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 11:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1345*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    case 12:
        iSetLineWidth(6);
        iSetTransparentColor(0,0,0,1);
        iRectangle(1616*ratio, 134*ratio, 260*ratio, 262*ratio);
        break;
    }
}


//Gambler Merchant Functions (Scene 14 : Level B1)
void dart_animate(int init)
{
    if(dart_counter > (init + 90)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart1);
    else if(dart_counter > (init + 80)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart2);
    else if(dart_counter > (init + 70)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart1);
    else if(dart_counter > (init + 60)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart2);
    else if(dart_counter > (init + 50)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart1);
    else if(dart_counter > (init + 40)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart2);
    else if(dart_counter > (init + 00)) iShowLoadedImage((1065+aim_x)*ratio, (224+aim_y)*ratio, &dart1);
}

int dart_score_count(void)
{
    int temp_score = 0;
    r = (int) sqrt((346-aim_x)*(346-aim_x) + (346-aim_y)*(346-aim_y));
    aim_x -= 346;
    aim_y -= 346;
    deg = trunc(atan2(abs(aim_y),abs(aim_x)) * 180 / M_PI);
    if(aim_x < 0 && aim_y > 0) deg = 180 - deg;
    else if(aim_x < 0 && aim_y < 0) deg = 180 + deg;
    else if(aim_x > 0 && aim_y < 0) deg = 360 - deg;
    iPlaySound("assets/sounds/dart.wav",false,100);
    if(r > 284) temp_score += 0;
    else if(r > 12 && r <= 27) temp_score += 25;
    else if(r > 0 && r <= 12) temp_score += 50;
    else
    {
        //find point mark
        deg = (deg+9)/18;
        point = dart_point[deg];
        if(r >261 && r <= 284) temp_score += point*2;
        else if(r >173 && r <= 261) temp_score += point;
        else if(r >151 && r <= 173) temp_score += point*3;
        else if(r >27 && r <= 151) temp_score += point;
    }

    return temp_score;
}

void get_score(void)
{
    iSetColor(0,0,0);
    char current_score[] = "Score: 00000";
    int temp_score = score;
    current_score[11] = temp_score%10 + '0';
    temp_score /= 10;
    current_score[10] = temp_score%10 + '0';
    temp_score /= 10;
    current_score[9] = temp_score%10 + '0';
    temp_score /= 10;
    current_score[8] = temp_score%10 + '0';
    temp_score /= 10;
    current_score[7] = temp_score%10 + '0';
    temp_score /= 10;
    iText(20*ratio, 1040*ratio, current_score, GLUT_BITMAP_HELVETICA_18);
}


void iDraw()
{
    iClear();
    //Full screen
    if(init == false) init = true, iToggleFullscreen();

    switch(scene)
    {
    case 0:
        iShowLoadedImage(0,0,&cover);
        break;
    case 1:
        //if(menu_bgm_status == false) menu_bgm_status = true, iPlaySound("assets/sound/main_bgm.wav", true, 20);
        iShowLoadedImage(0,0,&menu);
        if(save == false) iSetTransparentColor(0,0,0,0.6);
        else iSetColor(0,0,0);
        iText(500*ratio, 760*ratio, "CONTINUE", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0,0,0);
        iText(500*ratio, 700*ratio, "NEW GAME", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(500*ratio, 640*ratio, "LEADERBOARD", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(500*ratio, 580*ratio, "INSTRUCTION", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(500*ratio, 520*ratio, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);

        switch(sub_scene)
        {
        case 1:
            iSetColor(255,255,255);
            iFilledRectangle(480*ratio, 745*ratio, 312.5*ratio, 50*ratio);
            iSetColor(0,0,0);
            iText(500*ratio, 760*ratio, "CONTINUE", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 2:
            iSetColor(255,255,255);
            iFilledRectangle(480*ratio, 685*ratio, 312.5*ratio, 50*ratio);
            iSetColor(0,0,0);
            iText(500*ratio, 700*ratio, "NEW GAME", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 3:
            iSetColor(255,255,255);
            iFilledRectangle(480*ratio, 625*ratio, 312.5*ratio, 50*ratio);
            iSetColor(0,0,0);
            iText(500*ratio, 640*ratio, "LEADERBOARD", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 4:
            iSetColor(255,255,255);
            iFilledRectangle(480*ratio, 565*ratio, 312.5*ratio, 50*ratio);
            iSetColor(0,0,0);
            iText(500*ratio, 580*ratio, "INSTRUCTION", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 5:
            iSetColor(255,255,255);
            iFilledRectangle(480*ratio, 505*ratio, 312.5*ratio, 50*ratio);
            iSetColor(0,0,0);
            iText(500*ratio, 520*ratio, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        }
        break;
    case 2:
        iShowLoadedImage(0,0,&newGame);
        iSetColor(255,255,255);
        iTextBold(420*ratio, 740*ratio, "Start Your Rescue Mission", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetTransparentColor(255,255,255,0.8);
        iText(420*ratio, 700*ratio, "Input a name with maximum 30 characters.", GLUT_BITMAP_HELVETICA_18);
        iSetColor(255,255,255);
        iSetLineWidth(2);
        iRectangle(420*ratio, 630*ratio, 650*ratio, 50*ratio);
        iText(430*ratio, 645*ratio, name, GLUT_BITMAP_TIMES_ROMAN_24);
        iFilledRectangle(970*ratio, 580*ratio, 100*ratio, 40*ratio);
        iSetColor(0,0,0);
        iText(985*ratio, 590*ratio, "START", GLUT_BITMAP_HELVETICA_18);
        break;
    case 3:
        iShowLoadedImage(0,0,&leaderboard);
        iSetColor(0,0,0);
        iTextBold(420*ratio, 740*ratio, "Master Rescuers", GLUT_BITMAP_TIMES_ROMAN_24);

        score_ptr = fopen("saves/score.txt", "r");

        iSetColor(0,0,0);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(420*ratio, 660*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(950*ratio,660*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        iSetColor(255,255,255);
        iFilledRectangle(410*ratio,600*ratio,600*ratio,40*ratio);
        iSetColor(0,0,0);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(420*ratio, 610*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(950*ratio,610*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(420*ratio, 560*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(950*ratio,560*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        iSetColor(255,255,255);
        iFilledRectangle(410*ratio,500*ratio,600*ratio,40*ratio);
        iSetColor(0,0,0);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(420*ratio, 510*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(950*ratio,510*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(420*ratio, 460*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);
        fscanf(score_ptr, "%s\n", stringRead);
        iText(950*ratio,460*ratio, stringRead, GLUT_BITMAP_HELVETICA_18);

        fclose(score_ptr);
        break;
    case 4:
        iShowLoadedImage(0,0,&instruction);
        switch(sub_scene)
        {
        case 0:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Getting Started", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "Welcome our HERO. You have a village to save. Let's get some important", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "instruction that will help you in this journey.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 1:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Movement", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "In the open world, you can move around by pressing \"W\", \"A\", \"S\",", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "\"D\".", GLUT_BITMAP_HELVETICA_18);
            break;
        case 2:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Rescue Mission", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "All the house represent a rescuing mission. When nearby, you can press", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "\"Q\" to see details and press \"E\" to play the mission. But every", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 620*ratio, "mission requires some tool. Find them by completing other mission.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 3:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Puzzles", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "Rescuing someone requires solving a puzzle. Every puzzle has their own", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "instruction. Also you can find key functions at the bottom left.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 4:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Time Limit", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "Each person you are going to rescue might die if you can't save them", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "timely. If they die, you can still complete the puzzle and earn the", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 620*ratio, "reward tool. But it will reduce your score significantly.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 5:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Pause and Save", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "You can press \"Esc\" key to pause the game. When in a puzzle, exit from", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "that to pause the timer. Pause menu will not be shown there. This game", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 620*ratio, "saves your data automatically. You can resume even if you close the game.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 6:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "Bonus Task", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "There might be some bonus task. They require no tool. Completing them", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "can increase your score.", GLUT_BITMAP_HELVETICA_18);
            break;
        case 7:
            iSetColor(255,255,255);
            iTextBold(400*ratio, 740*ratio, "That's All", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(400*ratio, 680*ratio, "There is no time to waste, our HERO. Lets start rescuing the poor", GLUT_BITMAP_HELVETICA_18);
            iText(400*ratio, 650*ratio, "villagers. Hail to our HERO.", GLUT_BITMAP_HELVETICA_18);
            break;
        }
        iSetColor(255,255,255);
        if(sub_scene != 0) iFilledRectangle(1080*ratio, 570*ratio, 30*ratio, 30*ratio);
        if(sub_scene != 7) iFilledRectangle(1120*ratio, 570*ratio, 30*ratio, 30*ratio);
        iSetColor(0,0,0);
        tri_x[0] = 1085*ratio, tri_x[1] = tri_x[2] = 1105*ratio, tri_y[0] = 585*ratio, tri_y[1] = 575*ratio, tri_y[2] = 595*ratio;
        if(sub_scene != 0) iFilledPolygon(tri_x,tri_y, 3);
        tri_x[0] = tri_x[1] = 1125*ratio, tri_x[2] = 1145*ratio, tri_y[0] = 575*ratio, tri_y[1] = 595*ratio, tri_y[2] = 585*ratio;
        if(sub_scene != 7) iFilledPolygon(tri_x,tri_y, 3);
        break;
    case 5:
        iStopSound(menu_bgm);
        menu_bgm_status = false;
        iSetColor(0,0,0);
        iFilledRectangle(0,0,1920*ratio, 1080*ratio);
        iSetColor(255,255,255);
        switch(sub_scene)
        {
        case 0:
            iTextBold(400*ratio, 700*ratio, "30 July 2025", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 650*ratio, "A powerful cyclone struck a remote village late last night, unleashing fierce winds and torrential rains", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 620*ratio, "that lasted for hours. The storm made landfall without much warning, catching many residents unprepared.", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 590*ratio, "Thatched houses were torn from their foundations, trees were uprooted, and power lines lay tangled on the", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 560*ratio, "ground. The narrow dirt roads  that connect the village to nearby towns were flooded and rendered ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 530*ratio, "impassable, effectively cutting off all outside communication and aid.", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 1:
            iTextBold(400*ratio, 650*ratio, "The destruction left behind is vast and heartbreaking. Farmlands that had been ready for harvest are now", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 620*ratio, "submerged under water, and livestock have either perished or gone missing. The local school building, ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 590*ratio, "which had served as an emergency shelter, collapsed under the pressure of the winds. Survivors are now", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 560*ratio, "left without food, clean water, or shelter, as rescue efforts struggle to reach the isolated area. The ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 530*ratio, "once peaceful village now lies in ruins, its people facing an uncertain and difficult path to recovery.", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 2:
            iTextBold(400*ratio, 650*ratio, "Amid the chaos and devastation, a brave figure emerged. Though only a man with no special powers, he", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 620*ratio, "braved the storm’s fury to guide stranded villagers to safety and rescue the injured from beneath ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 590*ratio, "collapsed homes.  With nothing but courage in his heart, he became the unlikely hero his village ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 560*ratio, "desperately needed. ", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(400*ratio, 530*ratio, "Hail to our HERO. Hail to", GLUT_BITMAP_TIMES_ROMAN_24);
            iTextBold(735*ratio, 530*ratio, name , GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        }
        iText(850*ratio, 400*ratio, "Press \"Space\" to continue.", GLUT_BITMAP_HELVETICA_18);
        break;
    case 6:
        if(world_bgm_status == false) world_bgm_status = true, iPlaySound("assets/sounds/world_bgm.wav", true, 100);

        //Dynamic Movement
        if(x_position > 0)
        {
            if(y_position > 0) //hero left, down
            {
                iShowLoadedImage(0*ratio,0*ratio, &world);
                //iShowLoadedImage((936-x_position)*ratio,(522-y_position)*ratio, &hero1);
                heroMove((936-x_position)*ratio,(522-y_position)*ratio);
            }
            else if(y_position < -2160) //hero left, up
            {
                iShowLoadedImage(0*ratio,-2160*ratio, &world);
                //iShowLoadedImage((936-x_position)*ratio,(522+(-2160-y_position))*ratio, &hero1);
                heroMove((936-x_position)*ratio,(522+(-2160-y_position))*ratio);
            }
            else //hero left
            {
                iShowLoadedImage(0*ratio,y_position*ratio, &world);
                //iShowLoadedImage((936-x_position)*ratio,522*ratio, &hero1);
                heroMove((936-x_position)*ratio,522*ratio);
            }
        }
        else if(x_position < -3840)
        {
            if(y_position > 0) //hero right, down
            {
                iShowLoadedImage(-3840*ratio,0*ratio, &world);
                //iShowLoadedImage((936-3840-x_position)*ratio,(522-y_position)*ratio, &hero1);
                heroMove((936-3840-x_position)*ratio,(522-y_position)*ratio);
            }
            else if(y_position < -2160) //hero right, up
            {
                iShowLoadedImage(-3840*ratio,-2160*ratio, &world);
                //iShowLoadedImage((936-3840-x_position)*ratio,(522-2160-y_position)*ratio, &hero1);
                heroMove((936-3840-x_position)*ratio,(522-2160-y_position)*ratio);
            }
            else //hero right
            {
                iShowLoadedImage(-3840*ratio,y_position*ratio, &world);
                //iShowLoadedImage((936-3840-x_position)*ratio,522*ratio, &hero1);
                heroMove((936-3840-x_position)*ratio,522*ratio);
            }
        }
        else
        {
            if(y_position > 0) //hero down
            {
                iShowLoadedImage(x_position*ratio,0*ratio, &world);
                //iShowLoadedImage(936*ratio,(522-y_position)*ratio, &hero1);
                heroMove(936*ratio,(522-y_position)*ratio);
            }
            else if(y_position < -2160) //hero up
            {
                iShowLoadedImage(x_position*ratio,-2160*ratio, &world);
                //iShowLoadedImage(936*ratio,(522-2160-y_position)*ratio, &hero1);
                heroMove(936*ratio,(522-2160-y_position)*ratio);
            }
            else //hero normal
            {
                iShowLoadedImage(x_position*ratio,y_position*ratio, &world);
                //iShowLoadedImage(936*ratio,522*ratio, &hero1);
                heroMove(936*ratio,522*ratio);
            }
        }

        iSetColor(0,0,0);
        getPosition();
        //iText(20,30,x_pos,GLUT_BITMAP_HELVETICA_18);
        //iText(20,10,y_pos,GLUT_BITMAP_HELVETICA_18);

        if(sub_scene >= 1 && sub_scene <= 9)
        {
        iSetColor(255,255,255);
        iFilledRectangle(10*ratio,10*ratio,60*ratio, 40*ratio);
        if(sub_scene <= 7) iFilledRectangle(170*ratio,10*ratio,85*ratio, 40*ratio);
        iSetColor(0,0,0);
        iTextBold(20*ratio,20*ratio, "Esc", GLUT_BITMAP_HELVETICA_18);
        iText(90*ratio,20*ratio, "Back", GLUT_BITMAP_HELVETICA_18);
        if(sub_scene <= 7) iTextBold(180*ratio,20*ratio, "Space", GLUT_BITMAP_HELVETICA_18);
        if(sub_scene <= 7) iText(260*ratio,20*ratio, "Turn Over", GLUT_BITMAP_HELVETICA_18);
        }

        switch(sub_scene)
        {
        case 0:
            //Show Score
            get_score();
            if(current_ineteraction != -1)
            {
            iSetColor(255,255,255);
            iFilledRectangle(10*ratio,10*ratio,40*ratio, 40*ratio);
            iSetColor(0,0,0);
            iTextBold(20*ratio,20*ratio, "Q", GLUT_BITMAP_HELVETICA_18);
            iText(70*ratio,20*ratio, "Details", GLUT_BITMAP_HELVETICA_18);
            }

            if(current_ineteraction == current_level || current_ineteraction >= 8)
            {
            iSetColor(255,255,255);
            iFilledRectangle(150*ratio,10*ratio,35*ratio, 40*ratio);
            iSetColor(0,0,0);
            iTextBold(160*ratio,20*ratio, "E", GLUT_BITMAP_HELVETICA_18);
            iText(200*ratio,20*ratio, "Interact", GLUT_BITMAP_HELVETICA_18);
            }
            break;
        case 1:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/bh1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/bh2.png");
            break;
        case 2:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/cf1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/cf2.png");
            break;
        case 3:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/dl1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/dl2.png");
            break;
        case 4:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/e1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/e2.png");
            break;
        case 5:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/lb1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/lb2.png");
            break;
        case 6:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/ltl1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/ltl2.png");
            break;
        case 7:
            if(card_toggle == false) iShowImage(710*ratio, 390*ratio, "assets/images/cards/dyc1.png");
            else iShowImage(710*ratio, 390*ratio, "assets/images/cards/dyc2.png");
            break;
        case 8:
            iShowImage(710*ratio, 390*ratio, "assets/images/cards/gm.png");
            break;
        case 9:
            iShowImage(710*ratio, 390*ratio, "assets/images/cards/gpc.png");
            break;
        case 10:
            iShowLoadedImage(350*ratio, 600*ratio, &pass);
            iSetColor(0,0,0);
            //char new_score[11] = "Score +";
            //if(added_score%10 >=0 && added_score%10 <= 9) new_score[10] = added_score%10 + '0', added_score /= 10;
            //if(added_score%10 >=0 && added_score%10 <= 9) new_score[9] = added_score%10 + '0', added_score /= 10;
            //if(added_score%10 >=0 && added_score%10 <= 9) new_score[8] = added_score%10 + '0', added_score /= 10;
            //if(added_score%10 >=0 && added_score%10 <= 9) new_score[7] = added_score%10 + '0', added_score /= 10;
            //iText(900*ratio, 550*ratio, "Score +877", GLUT_BITMAP_TIMES_ROMAN_24);
            if(counter == 0) iPlaySound("assets/sounds/passed.wav", false, 100);
            if( counter != 600) counter++;
            else sub_scene = 0, counter = 0;
            break;
        case 11:
            iSetTransparentColor(0,0,0,0.3);
            iFilledRectangle(0,0,1920*ratio,1080*ratio);
            iSetColor(161,98,34);
            iFilledRectangle(710*ratio, 390*ratio, 500*ratio,300*ratio);
            iSetColor(115,48,7);
            iSetLineWidth(8);
            iRectangle(710*ratio, 390*ratio, 500*ratio,300*ratio);
            //iFilledRectangle(740*ratio,570*ratio,400*ratio, 40*ratio);
            //iFilledRectangle(740*ratio,520*ratio,400*ratio, 40*ratio);
            //iFilledRectangle(740*ratio,470*ratio,400*ratio, 40*ratio);
            iSetColor(0,0,0);
            iText(750*ratio, 580*ratio, "RESUME", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(750*ratio, 530*ratio, "EXIT TO MAIN MENU", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(750*ratio, 480*ratio, "EXIT TO DESKTOP", GLUT_BITMAP_TIMES_ROMAN_24);
            break;
        case 12:
            iShowLoadedImage(350*ratio, 600*ratio, &fail);
            iSetColor(0,0,0);
            if( counter != 600) counter++;
            else sub_scene = 0, counter = 0;
            break;

        }
        break;
    case 7:
        //Burning House
        iShowLoadedImage(0,0,&bh);

        //Hover Effect
        iSetColor(255,255,255);
        iSetLineWidth(2);
        if(sub_focus == 1) iRectangle(866*ratio,16*ratio,100*ratio,1048*ratio);
        else if(sub_focus == 2) iRectangle(1081*ratio,16*ratio,100*ratio,1048*ratio);
        else if(sub_focus == 3) iRectangle(1296*ratio,16*ratio,100*ratio,1048*ratio);
        else if(sub_focus == 4) iRectangle(1511*ratio,16*ratio,100*ratio,1048*ratio);
        else if(sub_focus == 5) iRectangle(1726*ratio,16*ratio,100*ratio,1048*ratio);

        //Joints
        iSetColor(142,21,21);
        iFilledRectangle(856*ratio,789*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1071*ratio,730*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1071*ratio,30*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1286*ratio,906*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1501*ratio,828*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1501*ratio,192*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1716*ratio,789*ratio,120*ratio, 100*ratio);
        iFilledRectangle(1716*ratio,66*ratio,120*ratio, 100*ratio);

        //Indicator
        iSetColor(33,174,23);
        switch(current_valve)
        {
        case 5:
            iFilledRectangle(906*ratio, 629*ratio, 20*ratio, 20*ratio);
        case 4:
            iFilledRectangle(1551*ratio, 629*ratio, 20*ratio, 20*ratio);
        case 3:
            iFilledRectangle(1766*ratio, 629*ratio, 20*ratio, 20*ratio);
        case 2:
            iFilledRectangle(1121*ratio, 629*ratio, 20*ratio, 20*ratio);
        case 1:
            iFilledRectangle(1336*ratio, 629*ratio, 20*ratio, 20*ratio);
        }

        //Rotate
        if(rotate_count != 0)
        {
            if(rotate_count%10 == 0) (sub_rotate_count == 1) ? sub_rotate_count = 0 : sub_rotate_count = 1;

            if(focus == 1) (sub_rotate_count == 1) ? iShowLoadedImage(825*ratio,406*ratio,&valve2) : iShowLoadedImage(825*ratio,406*ratio,&valve1);
            else if(focus == 2) (sub_rotate_count == 1) ? iShowLoadedImage(1040*ratio,406*ratio,&valve2) : iShowLoadedImage(1040*ratio,406*ratio,&valve1);
            else if(focus == 3) (sub_rotate_count == 1) ? iShowLoadedImage(1255*ratio,406*ratio,&valve2) : iShowLoadedImage(1255*ratio,406*ratio,&valve1);
            else if(focus == 4) (sub_rotate_count == 1) ? iShowLoadedImage(1470*ratio,406*ratio,&valve2) : iShowLoadedImage(1470*ratio,406*ratio,&valve1);
            else if(focus == 5) (sub_rotate_count == 1) ? iShowLoadedImage(1685*ratio,406*ratio,&valve2) : iShowLoadedImage(1685*ratio,406*ratio,&valve1);

            //Checking
            if(valve_order[current_valve] != focus)
            {
            if(rotate_count == 90) iPlaySound("assets/sounds/error.wav", false, 50);
            iSetColor(216,19,19);
            iFilledRectangle(906*ratio, 629*ratio, 20*ratio, 20*ratio);
            iFilledRectangle(1551*ratio, 629*ratio, 20*ratio, 20*ratio);
            iFilledRectangle(1766*ratio, 629*ratio, 20*ratio, 20*ratio);
            iFilledRectangle(1121*ratio, 629*ratio, 20*ratio, 20*ratio);
            iFilledRectangle(1336*ratio, 629*ratio, 20*ratio, 20*ratio);
            }
            rotate_count--;
        }
        if(rotate_count == 1)
        {
            if(valve_order[current_valve] != focus) current_valve = 0;
            else current_valve ++;
            focus = 0;
            rotate_count = 0;
            if(current_valve == 5) win = true;
        }

        //Valves
        if(focus != 1) iShowLoadedImage(825*ratio,406*ratio,&valve1);
        if(focus != 2) iShowLoadedImage(1040*ratio,406*ratio,&valve1);
        if(focus != 3) iShowLoadedImage(1255*ratio,406*ratio,&valve1);
        if(focus != 4) iShowLoadedImage(1470*ratio,406*ratio,&valve1);
        if(focus != 5) iShowLoadedImage(1685*ratio,406*ratio,&valve1);

        //winning
        if(win == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/win.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 10;
                win = false;
                score += score_list[current_level-1];
                if(level_timer != 0) score += (int)(1000*(level_timer/(double)level_timer_list[current_level-1]));
                //score += added_score;
                current_level++;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
            }
        }
        else
        {
            //timer decreases
            if(win == false && level_timer > 0) level_timer--;
            if(win == false && level_timer == 1) iPlaySound("assets/sounds/fail.wav", false, 100);

        }

        //timer bar render
        iSetColor(97,59,20);
        iFilledRectangle(171*ratio, 192*ratio, 563*ratio, 10*ratio);
        iSetColor(0,0,0);
        iFilledRectangle(171*ratio, 192*ratio, 563*(level_timer/(double)level_timer_list[current_level-1])*ratio, 10*ratio);
        if(level_timer == 0)
        {
            iShowLoadedImage(94*ratio, 154*ratio, &dead);
            iSetColor(161,98,34);
            iFilledRectangle(171*ratio, 143*ratio, 563*ratio, 39*ratio);
            iSetColor(0,0,0);
            iText(175*ratio, 160*ratio, "The person is dead :(", GLUT_BITMAP_HELVETICA_18);
        }
        break;
    case 8:
        //Cracked Floor
        iShowLoadedImage(0,0,&cf);
        //Hover
        iSetColor(0,0,0);
        iSetLineWidth(2);
        if(sub_focus == 133) iRectangle(317*ratio, 343*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 134) iRectangle(429*ratio, 343*ratio, 80*ratio, 80*ratio);
        else if(sub_focus != 0) iSetColor(255,255,255), iRectangle((796+((sub_focus-1)%12)*92)*ratio, (968-((sub_focus-1)/12)*94)*ratio, 80*ratio, 80*ratio);

        //Tools
        iSetColor(0,0,0);
        if(tool == false) iRectangle(317*ratio, 343*ratio, 80*ratio, 80*ratio);
        else iRectangle(429*ratio, 343*ratio, 80*ratio, 80*ratio);

        //Click Handle
        if(tool_map[(focus-1)/12][(focus-1)%12] == 0 && tool == true && focus != 0) tool_map[(focus-1)/12][(focus-1)%12] = 2, (floor_map[(focus-1)/12][(focus-1)%12] == -1) ? iPlaySound("assets/sounds/crack.wav", false, 100),crack_count++ : iPlaySound("assets/sounds/step.wav", false, 100), step_count++;
        else if(tool_map[(focus-1)/12][(focus-1)%12] == 0 && tool == false && focus != 0) tool_map[(focus-1)/12][(focus-1)%12] = 1, iPlaySound("assets/sounds/mark.wav", false, 100), marker_count++;
        else if(tool_map[(focus-1)/12][(focus-1)%12] == 1 && tool == false && focus != 0) tool_map[(focus-1)/12][(focus-1)%12] = 0, iPlaySound("assets/sounds/mark.wav", false, 100), marker_count--;
        focus = 0;

        //Floor Map
        for(int i=0; i<132; i++)
        {
            if(tool_map[i/12][i%12] == 2)
            {
                //when stepped
                switch(floor_map[i/12][i%12])
                {
                case -1:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &cracked);
                    break;
                case 0:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &zero);
                    break;
                case 1:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &one);
                    break;
                case 2:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &two);
                    break;
                case 3:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &three);
                    break;
                case 4:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &four);
                    break;
                case 5:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &five);
                    break;
                case 6:
                    iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &six);
                    break;
                }
            }
            else if(tool_map[i/12][i%12] == 1)
            {
                //when marked
                iShowLoadedImage((796+i%12*92)*ratio,(968-i/12*94)*ratio, &marked);
            }
        }

        if(step_count + marker_count + crack_count == 131) win = true;
        else if(crack_count == 3) failed = true;

        if(win == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/win.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 10;
                win = false;
                score += score_list[current_level-1];
                if(level_timer != 0) score += (int)(1000*(level_timer/(double)level_timer_list[current_level-1]));
                //score += added_score;
                current_level++;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
            }
        }
        else if(failed == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/fail.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 12;
                failed = false;
                step_count = 0;
                marker_count = 0;
                crack_count = 0;
                tool = false;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
                for(int i=0; i<132; i++) tool_map[i/12][i%12] = 0;
                tool_map[6][6] = 2;
            }
        }
        else
        {
            //timer decreases
            if(win == false && level_timer > 0) level_timer--;
            if(win == false && level_timer == 1) iPlaySound("assets/sounds/fail.wav", false, 100);

        }

        //timer bar render
        iSetColor(97,59,20);
        iFilledRectangle(171*ratio, 192*ratio, 563*ratio, 10*ratio);
        iSetColor(0,0,0);
        iFilledRectangle(171*ratio, 192*ratio, 563*(level_timer/(double)level_timer_list[current_level-1])*ratio, 10*ratio);
        if(level_timer == 0)
        {
            iShowLoadedImage(94*ratio, 154*ratio, &dead);
            iSetColor(161,98,34);
            iFilledRectangle(171*ratio, 143*ratio, 563*ratio, 39*ratio);
            iSetColor(0,0,0);
            iText(175*ratio, 160*ratio, "The person is dead :(", GLUT_BITMAP_HELVETICA_18);
        }
        break;
    case 9:
        //locked door
        iShowLoadedImage(0,0,&dl);
        //swapping cards
        if(prev_focus != focus && prev_focus != 0)
        {
            iPlaySound("assets/sounds/paper.wav", false, 100);
            int t = pieces[prev_focus-1];
            pieces[prev_focus-1] = pieces[focus-1];
            pieces[focus-1] = t;
            prev_focus = 0;
            focus = 0;
        }
        //setting pieces
        drawPiece(pieces[0], 797*ratio, 679*ratio);
        drawPiece(pieces[1], 1069*ratio, 679*ratio);
        drawPiece(pieces[2], 1341*ratio, 679*ratio);
        drawPiece(pieces[3], 1613*ratio, 679*ratio);
        drawPiece(pieces[4], 797*ratio, 405*ratio);
        drawPiece(pieces[5], 1069*ratio, 405*ratio);
        drawPiece(pieces[6], 1341*ratio, 405*ratio);
        drawPiece(pieces[7], 1613*ratio, 405*ratio);
        drawPiece(pieces[8], 797*ratio, 131*ratio);
        drawPiece(pieces[9], 1069*ratio, 131*ratio);
        drawPiece(pieces[10], 1341*ratio, 131*ratio);
        drawPiece(pieces[11], 1613*ratio, 131*ratio);
        pieceFocus();
        //check for win
        win = true;
        for(int i=1; i<=12; i++)
        {
            if(pieces[i-1] != i)
            {
                win = false;
                break;
            }
        }

        if(win == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/win.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 10;
                win = false;
                score += score_list[current_level-1];
                if(level_timer != 0) score += (int)(1000*(level_timer/(double)level_timer_list[current_level-1]));
                //score += added_score;
                current_level++;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
            }
        }
        else
        {
            //timer decreases
            if(win == false && level_timer > 0) level_timer--;
            if(win == false && level_timer == 1) iPlaySound("assets/sounds/fail.wav", false, 100);

        }

        //timer bar render
        iSetColor(97,59,20);
        iFilledRectangle(171*ratio, 192*ratio, 563*ratio, 10*ratio);
        iSetColor(0,0,0);
        iFilledRectangle(171*ratio, 192*ratio, 563*(level_timer/(double)level_timer_list[current_level-1])*ratio, 10*ratio);
        if(level_timer == 0)
        {
            iShowLoadedImage(94*ratio, 154*ratio, &dead);
            iSetColor(161,98,34);
            iFilledRectangle(171*ratio, 143*ratio, 563*ratio, 39*ratio);
            iSetColor(0,0,0);
            iText(175*ratio, 160*ratio, "The person is dead :(", GLUT_BITMAP_HELVETICA_18);
        }
        //printf("%d ", level_timer);
        break;
    case 10:
        iShowLoadedImage(0,0,&e);
        break;
    case 11:
        iShowLoadedImage(0,0,&lb);
        break;
    case 12:
        //Let There be Light
        iShowLoadedImage(0,0,&ltl);

        //Hover
        iSetColor(0,0,0);
        iSetLineWidth(2);
        if(sub_focus == 133) iRectangle(94*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 134) iRectangle(186*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 135) iRectangle(278*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 136) iRectangle(370*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 137) iRectangle(462*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 138) iRectangle(554*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus == 139) iRectangle(646*ratio, 552*ratio, 80*ratio, 80*ratio);
        else if(sub_focus != 0) iSetColor(255,255,255), iRectangle((796+((sub_focus-1)%12)*92)*ratio, (968-((sub_focus-1)/12)*94)*ratio, 80*ratio, 80*ratio);

        //Click
        if(focus != 0) last_bulb_idx = bulb_idx, bulb_idx = -1;

        if(focus != 0)
        {
            for(int i=0; i<7; i++)
            {
                if(bulb_pos[i][0] == 132-focus && focus >= 133)
                {
                    bulb_idx = i;
                    break;
                }
                else if(bulb_pos[i][0] == (focus-1)/12 && bulb_pos[i][1] == (focus-1)%12 && focus <133)
                {
                    bulb_idx = i;
                    break;
                }
            }
        }

        if(bulb_idx == -1 && last_bulb_idx != -1)
        {
            //printf("Swap\n", bulb_idx);
            if(focus < 133)
            {
                if(base_map[(focus-1)/12][(focus-1)%12] == 1) swapValid = false;
                else swapValid = true;
            }

            if(swapValid == true)
            {
                bulb_pos[last_bulb_idx][0] = (focus >= 133) ? 132-focus : (focus-1)/12;
                bulb_pos[last_bulb_idx][1] = (focus >= 133) ? 132-focus : (focus-1)%12;
                iPlaySound("assets/sounds/light.wav", false, 100);
            }
            bulb_idx = -1;
            last_bulb_idx = -1;
        }
        else if(bulb_idx != -1)
        {
            iSetColor(0,0,0);
            switch(bulb_pos[bulb_idx][0])
            {
            case -1:
                iRectangle(94*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -2:
                iRectangle(186*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -3:
                iRectangle(278*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -4:
                iRectangle(370*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -5:
                iRectangle(462*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -6:
                iRectangle(554*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            case -7:
                iRectangle(646*ratio, 552*ratio, 80*ratio, 80*ratio);
                break;
            default:
                iRectangle((796+bulb_pos[bulb_idx][1]*92)*ratio, (968-bulb_pos[bulb_idx][0]*94)*ratio, 80*ratio, 80*ratio);
                break;
            }
        }
        focus = 0;

        //Light
        for(int i=0; i<7; i++)
        {
            if(bulb_pos[i][0] >= 0)
            {
                //left
                while(base_map[bulb_pos[i][0]][bulb_pos[i][1]-bulb_i] != 1 && bulb_pos[i][1]-bulb_i != -1) base_map[bulb_pos[i][0]][bulb_pos[i][1]-bulb_i] = 2, bulb_i++;
                bulb_i = 0;
                //right
                while(base_map[bulb_pos[i][0]][bulb_pos[i][1]+bulb_i] != 1 && bulb_pos[i][1]+bulb_i != 12) base_map[bulb_pos[i][0]][bulb_pos[i][1]+bulb_i] = 2, bulb_i++;
                bulb_i = 0;
                //top
                while(base_map[bulb_pos[i][0]-bulb_i][bulb_pos[i][1]] != 1 && bulb_pos[i][0]-bulb_i != -1) base_map[bulb_pos[i][0]-bulb_i][bulb_pos[i][1]] = 2, bulb_i++;
                bulb_i = 0;
                //bottom
                while(base_map[bulb_pos[i][0]+bulb_i][bulb_pos[i][1]] != 1 && bulb_pos[i][0]+bulb_i != 11) base_map[bulb_pos[i][0]+bulb_i][bulb_pos[i][1]] = 2, bulb_i++;
                bulb_i = 0;
            }
        }
        iSetTransparentColor(255,213,0,0.4);
        for(int i=0; i<11; i++)
        {
            for(int j=0; j<12; j++)
            {
                if(base_map[i][j] == 2)iFilledRectangle((796+j*92)*ratio, (968-i*94)*ratio, 80*ratio, 80*ratio), base_map[i][j] = 0, light_tile++;
            }
        }


        //Bulbs
        iSetColor(0,0,0);
        for(int i=0; i<7; i++)
        {
            if(bulb_pos[i][0] == -1) iShowLoadedImage(94*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -2) iShowLoadedImage(186*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -3) iShowLoadedImage(278*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -4) iShowLoadedImage(370*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -5) iShowLoadedImage(462*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -6) iShowLoadedImage(554*ratio, 552*ratio, &bulb);
            else if(bulb_pos[i][0] == -7) iShowLoadedImage(646*ratio, 552*ratio, &bulb);
            else iShowLoadedImage((796+bulb_pos[i][1]*92)*ratio, (968 - bulb_pos[i][0]*94)*ratio, &bulb);
        }


        if(light_tile == (132-44)) win = true;
        else light_tile = 0;

        if(win == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/win.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 10;
                win = false;
                score += score_list[current_level-1];
                if(level_timer != 0) score += (int)(1000*(level_timer/(double)level_timer_list[current_level-1]));
                //score += added_score;
                current_level++;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
            }
        }
        else
        {
            //timer decreases
            if(win == false && level_timer > 0) level_timer--;
            if(win == false && level_timer == 1) iPlaySound("assets/sounds/fail.wav", false, 100);

        }

        //timer bar render
        iSetColor(97,59,20);
        iFilledRectangle(171*ratio, 192*ratio, 563*ratio, 10*ratio);
        iSetColor(0,0,0);
        iFilledRectangle(171*ratio, 192*ratio, 563*(level_timer/(double)level_timer_list[current_level-1])*ratio, 10*ratio);
        if(level_timer == 0)
        {
            iShowLoadedImage(94*ratio, 154*ratio, &dead);
            iSetColor(161,98,34);
            iFilledRectangle(171*ratio, 143*ratio, 563*ratio, 39*ratio);
            iSetColor(0,0,0);
            iText(175*ratio, 160*ratio, "The person is dead :(", GLUT_BITMAP_HELVETICA_18);
        }

        break;
    case 13:
        //Do You Copy
        iShowLoadedImage(0,0,&dyc);

        //Toggle Button
        iSetColor(77, 139, 10);
        if(radio_button[0] == 1) iFilledRectangle(1112*ratio, 442*ratio,45*ratio, 21*ratio);
        if(radio_button[1] == 1) iFilledRectangle(1180*ratio, 442*ratio,45*ratio, 21*ratio);
        if(radio_button[2] == 1) iFilledRectangle(1248*ratio, 442*ratio,45*ratio, 21*ratio);
        if(radio_button[3] == 1) iFilledRectangle(1316*ratio, 442*ratio,45*ratio, 21*ratio);
        if(radio_button[4] == 1) iFilledRectangle(1385*ratio, 442*ratio,45*ratio, 21*ratio);
        if(radio_button[5] == 1) iFilledRectangle(1453*ratio, 442*ratio,45*ratio, 21*ratio);

        iSetColor(255,255,255);
        iSetLineWidth(2);
        //Hover Knobs & Buttons
        if(sub_focus == 1) iCircle(1005.6*ratio, 441.5*ratio, 40.5*ratio, 100);
        else if(sub_focus == 2) iCircle(1628.5*ratio, 441.5*ratio, 40.5*ratio, 100);
        else if(sub_focus == 3) iRectangle(1112*ratio, 442*ratio,45*ratio, 21*ratio);
        else if(sub_focus == 4) iRectangle(1180*ratio, 442*ratio,45*ratio, 21*ratio);
        else if(sub_focus == 5) iRectangle(1248*ratio, 442*ratio,45*ratio, 21*ratio);
        else if(sub_focus == 6) iRectangle(1316*ratio, 442*ratio,45*ratio, 21*ratio);
        else if(sub_focus == 7) iRectangle(1385*ratio, 442*ratio,45*ratio, 21*ratio);
        else if(sub_focus == 8) iRectangle(1453*ratio, 442*ratio,45*ratio, 21*ratio);

        //Dots
        iSetColor(255,255, 255);
        //left knob
        if(radio_knob1 == 0) iFilledCircle(1006*ratio, 477*ratio, 4*ratio, 100);
        else if(radio_knob1 == 1) iFilledCircle(1032*ratio, 466*ratio, 4*ratio, 100);
        else if(radio_knob1 == 2) iFilledCircle(1041*ratio, 441*ratio, 4*ratio, 100);
        else if(radio_knob1 == 3) iFilledCircle(1032*ratio, 416*ratio, 4*ratio, 100);
        else if(radio_knob1 == 4) iFilledCircle(1006*ratio, 406*ratio, 4*ratio, 100);
        else if(radio_knob1 == 5) iFilledCircle(981*ratio, 416*ratio, 4*ratio, 100);
        else if(radio_knob1 == 6) iFilledCircle(970*ratio, 441*ratio, 4*ratio, 100);
        else if(radio_knob1 == 7) iFilledCircle(981*ratio, 466*ratio, 4*ratio, 100);
        //right knob
        if(radio_knob2 == 0) iFilledCircle(1629*ratio, 477*ratio, 4*ratio, 100);
        else if(radio_knob2 == 1) iFilledCircle(1655*ratio, 466*ratio, 4*ratio, 100);
        else if(radio_knob2 == 2) iFilledCircle(1664*ratio, 441*ratio, 4*ratio, 100);
        else if(radio_knob2 == 3) iFilledCircle(1655*ratio, 416*ratio, 4*ratio, 100);
        else if(radio_knob2 == 4) iFilledCircle(1629*ratio, 406*ratio, 4*ratio, 100);
        else if(radio_knob2 == 5) iFilledCircle(1604*ratio, 416*ratio, 4*ratio, 100);
        else if(radio_knob2 == 6) iFilledCircle(1593*ratio, 441*ratio, 4*ratio, 100);
        else if(radio_knob2 == 7) iFilledCircle(1604*ratio, 466*ratio, 4*ratio, 100);

        //indicator
        iSetColor(28,23,17);
        iFilledRectangle(975.5*ratio, 493.5*ratio, 48.5*ratio, 26*ratio);

        //combination calcilation
        int r1, r2, b1, b2; //1 for right, 2 for left
        r1 = r2 = b1 = b2 = 0;
        (radio_knob1 > 4) ? r2 += radio_knob1 - 4 : r1 += radio_knob1;
        (radio_knob2 > 6) ? r2 += radio_knob2 - 6 : r1 += radio_knob2;
        if(radio_button[0] == 1) b2++;
        if(radio_button[1] == 1) b1++;
        if(radio_button[2] == 1) b2++;
        if(radio_button[3] == 1) b1++;
        if(radio_button[4] == 1) b2++;
        if(radio_button[5] == 1) b1++;
        iSetTransparentColor(255,0,0,0.9);
        cursor_pos = 242*(((r1+b1)/13.0)-((r2+b2)/7.0)) + 1314;
        iFilledRectangle(cursor_pos*ratio, 482*ratio, 11*ratio, 27*ratio);

        //winning
        if(r1+b1 == 13) win = true;
        if(win == true)
        {
            if(win_counter == 300)win_counter--, iPlaySound("assets/sounds/win.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                scene = 6;
                sub_scene = 10;
                win = false;
                score += score_list[current_level-1];
                current_level++;
                level_timer = level_timer_list[current_level-1];
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;

                //Game Finish
            }
        }
        break;
    case 14:
        //Gambler Merchant
        iShowLoadedImage(0,0,&gm);
        if(win == false)
        {
            iSetTransparentColor(0,0,255,0.7);
            if(dart_counter == 200) iFilledCircle((1075+aim_x)*ratio, (234+aim_y)*ratio, 10*ratio,100);
            //Aim X
            if(aim_x_dir == 1) aim_x += aim_move_speed;
            else if(aim_x_dir == -1) aim_x -= aim_move_speed;
            if(aim_x >= 691 && aim_x_dir != 0) aim_x_dir = -1;
            if(aim_x <= 0 && aim_x_dir != 0) aim_x_dir = 1;
            //Aim Y
            if(aim_x_dir == 0)
            {
                if(aim_y_dir == 1) aim_y += aim_move_speed;
                else if(aim_y_dir == -1) aim_y -= aim_move_speed;
                if(aim_y >= 691 && aim_y_dir != 0) aim_y_dir = -1;
                if(aim_y <= 0 && aim_y_dir != 0) aim_y_dir = 1;
            }

            //Score and Opponent

            //center 1420,580
            //polar coordinate
            if(aim_y_dir == 0)
            {
                //printf("\n%d, %d, %d", aim_x, aim_y, deg);

                //scoring
                if(dart_counter == 200)
                {
                    this_dart = dart_score_count();
                    dart_score += this_dart;
                    aim_x += 346;
                    aim_y += 346;
                }

                if(dart_counter != 0) dart_counter--;
                if(dart_counter > 100) dart_animate(100);

                //opponent


                if(dart_counter == 100)
                {
                    srand(time(NULL));
                    dart_random = rand()%20;
                    aim_x = 346+gamble_move_x[dart_random];
                    aim_y = 346+gamble_move_y[dart_random];
                    this_dart = dart_score_count();
                    merchant_score += this_dart;
                    aim_x += 346;
                    aim_y += 346;
                }

                if(dart_counter <= 100) dart_animate(0);
                if(dart_counter == 1)
                {
                    aim_x = 0;
                    aim_x_dir = 1;
                    aim_y = 346;
                    aim_y_dir = 1;
                    dart_counter = 200;
                    this_dart = 0;
                    if(dart_score >= 250 || merchant_score >= 250) win = true;
                }
            }
        }
        else
        {
            if(win_counter == 300)win_counter--, (dart_score >= 250) ? iPlaySound("assets/sounds/win.wav", false, 100) : iPlaySound("assets/sounds/fail.wav", false, 100);
            else if(win_counter != 0) win_counter--;
            else
            {
                //winning
                win = false;
                if(dart_score >= 250)
                {
                    scene = 6;
                    sub_scene = 10;
                    score += 1000;
                    bonus_one = true;
                }
                else
                {
                    scene = 6;
                    sub_scene = 12;
                }
                dart_score = 0;
                merchant_score = 0;
                //saveGame();
                focus = 0;
                sub_focus = 0;
                prev_focus = 0;
                win_counter = 300;
            }
        }

        iSetColor(0,0,0);
        //this dart's score
        if(this_dart > 0)
        {
            dart_score_s[2] = this_dart%10 + '0';
            dart_score_s[1] = (this_dart/10)%10 + '0';
            dart_score_s[0] = '+';
            iText(1394*ratio, 184*ratio, dart_score_s, GLUT_BITMAP_TIMES_ROMAN_24);
        }
        dart_score_s[2] = dart_score%10 + '0';
        dart_score_s[1] = (dart_score/10)%10 + '0';
        dart_score_s[0] = (dart_score/100)%10 + '0';
        iText(224*ratio, 380*ratio, dart_score_s, GLUT_BITMAP_TIMES_ROMAN_24);
        dart_score_s[2] = merchant_score%10 + '0';
        dart_score_s[1] = (merchant_score/10)%10 + '0';
        dart_score_s[0] = (merchant_score/100)%10 + '0';
        iText(500*ratio, 380*ratio, dart_score_s, GLUT_BITMAP_TIMES_ROMAN_24);
        break;
    case 15:
        iShowLoadedImage(0,0,&gpc);
        break;
    }
}



void iMouseMove(int mx, int my)
{
    switch(scene)
    {
    case 1:
        if(mx > 480*ratio && mx < 792.5*ratio)
        {
            if(my > 745*ratio && my < 795*ratio && save == true) prev_sub_scene = sub_scene, sub_scene = 1, (prev_sub_scene != sub_scene) ? iPlaySound("assets/sounds/hover.wav", false, 100) : sub_scene = sub_scene;
            else if(my > 685*ratio && my < 735*ratio) prev_sub_scene = sub_scene, sub_scene = 2, (prev_sub_scene != sub_scene) ? iPlaySound("assets/sounds/hover.wav", false, 100) : sub_scene = sub_scene;
            else if(my > 625*ratio && my < 675*ratio) prev_sub_scene = sub_scene, sub_scene = 3, (prev_sub_scene != sub_scene) ? iPlaySound("assets/sounds/hover.wav", false, 100) : sub_scene = sub_scene;
            else if(my > 565*ratio && my < 615*ratio) prev_sub_scene = sub_scene, sub_scene = 4, (prev_sub_scene != sub_scene) ? iPlaySound("assets/sounds/hover.wav", false, 100) : sub_scene = sub_scene;
            else if(my > 505*ratio && my < 555*ratio) prev_sub_scene = sub_scene, sub_scene = 5, (prev_sub_scene != sub_scene) ? iPlaySound("assets/sounds/hover.wav", false, 100) : sub_scene = sub_scene;
            else sub_scene = 0;
        }
        else sub_scene = 0;
        break;
    case 7:
        //Burning House
        if(my > 16*ratio && my < 1064*ratio && win == false && rotate_count == 0)
        {
            if(mx > 866*ratio && mx < 966*ratio) sub_focus = 1;
            else if(mx > 1081*ratio && mx < 1181*ratio) sub_focus = 2;
            else if(mx > 1296*ratio && mx < 1396*ratio) sub_focus = 3;
            else if(mx > 1511*ratio && mx < 1611*ratio) sub_focus = 4;
            else if(mx > 1726*ratio && mx < 1826*ratio) sub_focus = 5;
            else sub_focus = 0;
        }
        else sub_focus = 0;
        break;
    case 8:
        //Cracked Floor
        sub_focus = 0;
        for(int i=0;i<132; i++)
        {
            //iRectangle((796+(i%12)*92)*ratio, (968-(i/12)*94)*ratio, 80*ratio, 80*ratio);
            if(mx > (796+(i%12)*92)*ratio && mx < (796+(i%12)*92+80)*ratio && my > (968-(i/12)*94)*ratio && my < (968-(i/12)*94+80)*ratio)
            {
                sub_focus = i+1;
                break;
            }
        }

        if(my > 343*ratio && my < 423*ratio)
        {
            if(mx > 317*ratio && mx < 397*ratio) sub_focus = 133;
            else if(mx > 429*ratio && mx < 509*ratio) sub_focus = 134;
        }
        break;
    case 9:
        //Door Locked
        if(my > 679*ratio && my < 949*ratio && win == false)
        {
            if(mx > 797*ratio && mx < 1066*ratio) sub_focus = 1;
            else if(mx > 1069*ratio && mx < 1338*ratio) sub_focus = 2;
            else if(mx > 1341*ratio && mx < 1610*ratio) sub_focus = 3;
            else if(mx > 1613*ratio && mx < 1882*ratio) sub_focus = 4;
            else sub_focus = 0;
        }
        else if(my > 405*ratio && my < 675*ratio && win == false)
        {
            if(mx > 797*ratio && mx < 1066*ratio) sub_focus = 5;
            else if(mx > 1069*ratio && mx < 1338*ratio) sub_focus = 6;
            else if(mx > 1341*ratio && mx < 1610*ratio) sub_focus = 7;
            else if(mx > 1613*ratio && mx < 1882*ratio) sub_focus = 8;
            else sub_focus = 0;
        }
        else if(my > 131*ratio && my < 401*ratio && win == false)
        {
            if(mx > 797*ratio && mx < 1066*ratio) sub_focus = 9;
            else if(mx > 1069*ratio && mx < 1338*ratio) sub_focus = 10;
            else if(mx > 1341*ratio && mx < 1610*ratio) sub_focus = 11;
            else if(mx > 1613*ratio && mx < 1882*ratio) sub_focus = 12;
            else sub_focus = 0;
        }
        else sub_focus = 0;
        break;
    case 12:
        //Let There Be Light
        sub_focus = 0;
        for(int i=0;i<132; i++)
        {
            //iRectangle((796+(i%12)*92)*ratio, (968-(i/12)*94)*ratio, 80*ratio, 80*ratio);
            if(mx > (796+(i%12)*92)*ratio && mx < (796+(i%12)*92+80)*ratio && my > (968-(i/12)*94)*ratio && my < (968-(i/12)*94+80)*ratio)
            {
                sub_focus = i+1;
                break;
            }
        }

        if(my > 552*ratio && my < 632*ratio)
        {
            if(mx > 94*ratio && mx < (94+80)*ratio) sub_focus = 133;
            else if(mx > 186*ratio && mx < (186+80)*ratio) sub_focus = 134;
            else if(mx > 278*ratio && mx < (278+80)*ratio) sub_focus = 135;
            else if(mx > 370*ratio && mx < (370+80)*ratio) sub_focus = 136;
            else if(mx > 462*ratio && mx < (462+80)*ratio) sub_focus = 137;
            else if(mx > 554*ratio && mx < (554+80)*ratio) sub_focus = 138;
            else if(mx > 646*ratio && mx < (646+80)*ratio) sub_focus = 139;
        }
        break;
    case 13:
        //Do You Copy
        if(my > 401*ratio && my < 482*ratio && win == false)
        {
            if(mx > 965.1*ratio && mx < 1046.1*ratio) sub_focus = 1;
            else if(mx > 1588*ratio && mx < 1669*ratio) sub_focus = 2;
            else if(my > 442*ratio && my < 463*ratio)
            {
                if(mx > 1112*ratio && mx < 1157*ratio) sub_focus = 3;
                else if(mx > 1180*ratio && mx < 1225*ratio) sub_focus = 4;
                else if(mx > 1248*ratio && mx < 1293*ratio) sub_focus = 5;
                else if(mx > 1316*ratio && mx < 1361*ratio) sub_focus = 6;
                else if(mx > 1385*ratio && mx < 1430*ratio) sub_focus = 7;
                else if(mx > 1453*ratio && mx < 1498*ratio) sub_focus = 8;
                else sub_focus = 0;
            }
            else sub_focus = 0;
        }
        else sub_focus = 0;
        break;
    }
}

void iMouseDrag(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        switch(scene)
        {
        case 1:
            if(mx > 480*ratio && mx < 792.5*ratio)
            {
                if(my > 745*ratio && my < 795*ratio) iPlaySound("assets/sounds/click.wav", false, 100); //continue
                else if(my > 685*ratio && my < 735*ratio) iPlaySound("assets/sounds/click.wav", false, 100), name_cursor = strlen(name), scene = 2, sub_scene = 0; //New game
                else if(my > 625*ratio && my < 675*ratio) iPlaySound("assets/sounds/click.wav", false, 100), scene = 3, sub_scene = 0; //Leaderboard
                else if(my > 565*ratio && my < 615*ratio) iPlaySound("assets/sounds/click.wav", false, 100), scene = 4, sub_scene = 0; //Instruction
                else if(my > 505*ratio && my < 555*ratio) iPlaySound("assets/sounds/click.wav", false, 100), exit(0); //Exit
            }
            break;
        case 2:
            if(mx > 970*ratio && mx < 1070*ratio && my > 580*ratio && my < 620*ratio) newGameInit(), iPlaySound("assets/sounds/click.wav", false, 100);
            break;
        case 4:
            if(mx > 1080*ratio && mx < 1110*ratio && my > 570*ratio && my < 600*ratio && sub_scene != 0) sub_scene--, iPlaySound("assets/sounds/click.wav", false, 100);
            else if(mx > 1120*ratio && mx < 1150*ratio && my > 570*ratio && my < 600*ratio && sub_scene != 7) sub_scene++, iPlaySound("assets/sounds/click.wav", false, 100);
            break;
        case 6:
            switch(sub_scene)
            {
            case 11:
                if(mx > 740*ratio && mx < 1140*ratio)
                {
                    if(my > 570*ratio && my < 610*ratio) sub_scene = 0,iPlaySound("assets/sounds/click.wav", false, 100);
                    if(my > 520*ratio && my < 560*ratio) saveGame(),scene = 1,sub_scene = 0,iPlaySound("assets/sounds/click.wav", false, 100);
                    if(my > 470*ratio && my < 510*ratio) saveGame(),iPlaySound("assets/sounds/click.wav", false, 100), exit(0);
                }
            }
            break;
        case 7:
            //Burning House
            if(my > 16*ratio && my < 1064*ratio && win == false && rotate_count == 0)
            {
                if(mx > 866*ratio && mx < 966*ratio) focus = 1, rotate_count = 100, iPlaySound("assets/sounds/valve.wav", false, 100);
                else if(mx > 1081*ratio && mx < 1181*ratio) focus = 2, rotate_count = 100, iPlaySound("assets/sounds/valve.wav", false, 100);
                else if(mx > 1296*ratio && mx < 1396*ratio) focus = 3, rotate_count = 100, iPlaySound("assets/sounds/valve.wav", false, 100);
                else if(mx > 1511*ratio && mx < 1611*ratio) focus = 4, rotate_count = 100, iPlaySound("assets/sounds/valve.wav", false, 100);
                else if(mx > 1726*ratio && mx < 1826*ratio) focus = 5, rotate_count = 100, iPlaySound("assets/sounds/valve.wav", false, 100);
                else sub_focus = 0;
            }
            else sub_focus = 0;
            break;
        case 8:
            //Cracked Floor
            for(int i=0;i<132; i++)
            {
                //iRectangle((796+(i%12)*92)*ratio, (968-(i/12)*94)*ratio, 80*ratio, 80*ratio);
                if(mx > (796+(i%12)*92)*ratio && mx < (796+(i%12)*92+80)*ratio && my > (968-(i/12)*94)*ratio && my < (968-(i/12)*94+80)*ratio)
                {
                    focus = i+1;
                    break;
                }
            }

            if(my > 343*ratio && my < 423*ratio)
            {
                if(mx > 317*ratio && mx < 397*ratio) tool = false;
                else if(mx > 429*ratio && mx < 509*ratio) tool = true;
            }
            break;
        case 9:
            //Door Locked
            if(my > 679*ratio && my < 949*ratio && win == false)
            {
                if(mx > 797*ratio && mx < 1066*ratio) prev_focus = focus, focus = 1;
                else if(mx > 1069*ratio && mx < 1338*ratio) prev_focus = focus, focus = 2;
                else if(mx > 1341*ratio && mx < 1610*ratio) prev_focus = focus, focus = 3;
                else if(mx > 1613*ratio && mx < 1882*ratio) prev_focus = focus, focus = 4;
                else prev_focus = 0, focus = 0;
            }
            else if(my > 405*ratio && my < 675*ratio && win == false)
            {
                if(mx > 797*ratio && mx < 1066*ratio) prev_focus = focus, focus = 5;
                else if(mx > 1069*ratio && mx < 1338*ratio) prev_focus = focus, focus = 6;
                else if(mx > 1341*ratio && mx < 1610*ratio) prev_focus = focus, focus = 7;
                else if(mx > 1613*ratio && mx < 1882*ratio) prev_focus = focus, focus = 8;
                else prev_focus = 0, focus = 0;
            }
            else if(my > 131*ratio && my < 401*ratio && win == false)
            {
                if(mx > 797*ratio && mx < 1066*ratio) prev_focus = focus, focus = 9;
                else if(mx > 1069*ratio && mx < 1338*ratio) prev_focus = focus, focus = 10;
                else if(mx > 1341*ratio && mx < 1610*ratio) prev_focus = focus, focus = 11;
                else if(mx > 1613*ratio && mx < 1882*ratio) prev_focus = focus, focus = 12;
                else prev_focus = 0, focus = 0;
            }
            else focus = 0;
            break;
        case 12:
            //Let There Be Light
            for(int i=0;i<132; i++)
            {
                //iRectangle((796+(i%12)*92)*ratio, (968-(i/12)*94)*ratio, 80*ratio, 80*ratio);
                if(mx > (796+(i%12)*92)*ratio && mx < (796+(i%12)*92+80)*ratio && my > (968-(i/12)*94)*ratio && my < (968-(i/12)*94+80)*ratio)
                {
                    focus = i+1;
                    break;
                }
            }

            if(my > 552*ratio && my < 632*ratio)
            {
                if(mx > 94*ratio && mx < (94+80)*ratio) focus = 133;
                else if(mx > 186*ratio && mx < (186+80)*ratio) focus = 134;
                else if(mx > 278*ratio && mx < (278+80)*ratio) focus = 135;
                else if(mx > 370*ratio && mx < (370+80)*ratio) focus = 136;
                else if(mx > 462*ratio && mx < (462+80)*ratio) focus = 137;
                else if(mx > 554*ratio && mx < (554+80)*ratio) focus = 138;
                else if(mx > 646*ratio && mx < (646+80)*ratio) focus = 139;
            }
            break;
        case 13:
            //Do You Copy
            if(my > 401*ratio && my < 482*ratio)
            {
                if(mx > 965.1*ratio && mx < 1046.1*ratio) radio_knob1 = (radio_knob1+1)%8, iPlaySound("assets/sounds/radio.wav", false, 100);
                else if(mx > 1588*ratio && mx < 1669*ratio) radio_knob2 = (radio_knob2+1)%8, iPlaySound("assets/sounds/radio.wav", false, 100);
                else if(my > 442*ratio && my < 463*ratio)
                {
                    if(mx > 1112*ratio && mx < 1157*ratio) (radio_button[0] == 0) ? radio_button[0] = 1 : radio_button[0] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                    else if(mx > 1180*ratio && mx < 1225*ratio) (radio_button[1] == 0) ? radio_button[1] = 1 : radio_button[1] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                    else if(mx > 1248*ratio && mx < 1293*ratio) (radio_button[2] == 0) ? radio_button[2] = 1 : radio_button[2] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                    else if(mx > 1316*ratio && mx < 1361*ratio) (radio_button[3] == 0) ? radio_button[3] = 1 : radio_button[3] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                    else if(mx > 1385*ratio && mx < 1430*ratio) (radio_button[4] == 0) ? radio_button[4] = 1 : radio_button[4] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                    else if(mx > 1453*ratio && mx < 1498*ratio) (radio_button[5] == 0) ? radio_button[5] = 1 : radio_button[5] = 0, iPlaySound("assets/sounds/radio.wav", false, 100);
                }
            }
            break;
        case 14:
            if(aim_x_dir != 0) aim_x_dir = 0;
            else if(aim_y_dir != 0) aim_y_dir = 0;
            break;
        }
    }
}

void iMouseWheel(int dir, int mx, int my)
{

}

void iKeyboard(unsigned char key)
{
    switch(scene)
    {
    case 0:
        if(key == ' ') scene = 1, sub_scene = 0, iPlaySound("assets/sounds/click.wav", false, 100);
        break;
    case 2:
        if(key == 27) scene = 1, sub_scene = 0;
        if(name_cursor < 30)
        {
            if(key >= 'A' && key <= 'Z') name[name_cursor] = key, name_cursor++;
            else if(key >= 'a' && key <= 'z') name[name_cursor] = key-'a'+'A', name_cursor++;
            else if(key == ' ' || key == '_') name[name_cursor] = '_', name_cursor++;
        }
        if(key == 8 && name_cursor != 0) name_cursor--, name[name_cursor] = ' ';
        break;
    case 3:
        if(key == 27) scene = 1, sub_scene = 0;
        break;
    case 4:
        if(key == 27) scene = 1, sub_scene = 0;
        break;
    case 5:
        if(key == ' ')
        {
            if(sub_scene < 2) sub_scene++;
            else scene = 6, sub_scene = 0;

            iPlaySound("assets/sounds/click.wav", false, 100);
        }
        break;
    case 6:
        if(key == 'w'&& checkMove(x_position,y_position - move_speed)) y_position -= move_speed, interactionCheck(), direction = 0, last_key_press= 10;
        else if(key == 's' && checkMove(x_position,y_position + move_speed)) y_position += move_speed, interactionCheck(), direction = 2, last_key_press= 10;
        else if(key == 'd' && checkMove(x_position - move_speed, y_position)) x_position -= move_speed, interactionCheck(), direction = 3, last_key_press= 10;
        else if(key == 'a' && checkMove(x_position + move_speed, y_position)) x_position += move_speed, interactionCheck(), direction = 1, last_key_press= 10;
        else if(key == 'q' && current_ineteraction != -1) sub_scene = current_ineteraction, card_toggle = false, iPlaySound("assets/sounds/click.wav", false, 100);
        else if(key == 'e' && current_ineteraction > 0) scene = current_ineteraction + 6,sub_scene = 0, iPlaySound("assets/sounds/click.wav", false, 100);
        else if(key == ' ' && sub_scene >= 1 && sub_scene <=9) (card_toggle == false) ? card_toggle = true : card_toggle = false, iPlaySound("assets/sounds/click.wav", false, 100);
        else if(key == 27) (sub_scene == 0) ? sub_scene = 11 : sub_scene = 0, iPlaySound("assets/sounds/click.wav", false, 100);
        break;
    case 7:
        if(key == 'p') level_timer = 3000;
    }

    if(scene >= 7 && scene <= 15 && key == 27) scene = 6, sub_scene = 0, iPlaySound("assets/sounds/click.wav", false, 100);
}

void iSpecialKeyboard(unsigned char key)
{
    switch(scene)
    {
    case 4:
        if(key == GLUT_KEY_LEFT && sub_scene != 0) sub_scene--, iPlaySound("assets/sounds/click.wav", false, 100);
        else if(key == GLUT_KEY_RIGHT && sub_scene != 7) sub_scene++, iPlaySound("assets/sounds/click.wav", false, 100);
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    iInitializeSound();
    //Load Save Game
    save_ptr = fopen("saves/save.txt","r");
    fscanf(save_ptr, "%s\n", stringRead);
    if(strcmp(stringRead, "YES") == 0)
    {
        //Load Others
        save = true;
        fscanf(save_ptr, "%s\n", name);
        fscanf(save_ptr, "%d\n", &current_level);
        fscanf(save_ptr, "%d\n", &level_timer);
        fscanf(save_ptr, "%s\n", stringRead);
        if(strcmp(stringRead, "TRUE") == 0) bonus_one = true;
        fscanf(save_ptr, "%s\n", stringRead);
        if(strcmp(stringRead, "TRUE") == 0) bonus_two = true;
        fscanf(save_ptr, "%d\n", &score);
        //this line should be removed
        level_timer = level_timer_list[current_level-1];
        current_level = 1;
        win = false;
        printf("%s, %d, %d, %d, %d, %d", name, current_level, level_timer, bonus_one, bonus_two, score);
    }
    else
    {
        save = false;
    }
    fclose(save_ptr);
    //Load Image
    loadImages();
    //Load Sound
    //menu_bgm = iPlaySound("assets/sounds/main_bgm.wav",true, 20);
    iInitialize(1920, 1080, "Rescue Rush");
    return 0;
}
