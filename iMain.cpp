#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <windows.h>
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

Image cover,menu,newGame,leaderboard,instruction, world, pass, bh, cf, dl, e,lb, ltl, dyc, gm, gpc;
Image hero, hero1, hero2, hero3, hero4, hero5, hero6, hero7, hero8, hero9, hero10, hero11, hero12;
FILE *score_ptr, *save_ptr;
int menu_bgm, world_bgm;
bool menu_bgm_status = true;
bool world_bgm_status = false;

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
    iLoadImage(&pass, "assets/images/effects/pass.png");
    iScaleImage(&pass, ratio);
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
    level_timer = 0;
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
            iText(900*ratio, 550*ratio, "Score +877", GLUT_BITMAP_TIMES_ROMAN_24);
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

        }
        break;
    case 7:
        iShowLoadedImage(0,0,&bh);
        break;
    case 8:
        iShowLoadedImage(0,0,&cf);
        break;
    case 9:
        iShowLoadedImage(0,0,&dl);
        break;
    case 10:
        iShowLoadedImage(0,0,&e);
        break;
    case 11:
        iShowLoadedImage(0,0,&lb);
        break;
    case 12:
        iShowLoadedImage(0,0,&ltl);
        break;
    case 13:
        iShowLoadedImage(0,0,&dyc);
        break;
    case 14:
        iShowLoadedImage(0,0,&gm);
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
    menu_bgm = iPlaySound("assets/sounds/main_bgm.wav",true, 20);
    iInitialize(1920, 1080, "Rescue Rush");
    return 0;
}
