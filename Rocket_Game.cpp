/*********
STUDENT : SYED RAFEH HUSSAIN
STUDENT NUMBER : 22001502
SECTION : 01
HOMEWORK: #2
----------
PROBLEMS: 
----------
ADDITIONAL FEATURES: Score, Game can be played again after it is finished
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 300

#define TIMER_ON 1; // 0:disable timer, 1:enable timer

#define D2R 0.0174532;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int xM = 0; // Jet moving value
int yM = 0; // Missile moving value
int xP = 0; // Target moving value

int randomY = 0; // Random target position

int score = 0; // Total score

double timer_period = 5;

#define OFF 0 // Start Game
#define RUN 1 // Playing Game
#define OVER 2 // Game Over

int GameState = OFF; // Start Game

bool animation = false; //flag to show if missile is fired/active

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawJet() {

    {
        glColor3ub(25, 125, 15);
        glRectf(-10 + xM, -140, 10 + xM, -100); // jet body

        glColor3ub(0, 0, 255);
        glBegin(GL_TRIANGLES); // jet top
        glVertex2f(-10 + xM, -100);
        glVertex2f(10 + xM, -100);
        glVertex2f(0 + xM, -85);
        glEnd();

        vprint(-6 + xM, -136, GLUT_BITMAP_HELVETICA_18, "R"); // jet symbol

        glColor3ub(25, 125, 15);
        glBegin(GL_QUADS); // jet wings

        glVertex2f(10 + xM, -130);
        glVertex2f(10 + xM, -110);
        glVertex2f(25 + xM, -140);
        glVertex2f(25 + xM, -120);

        glVertex2f(-10 + xM, -130);
        glVertex2f(-10 + xM, -110);
        glVertex2f(-25 + xM, -140);
        glVertex2f(-25 + xM, -120);

        glEnd();

        glColor3ub(127, 94, 12); // jet windows
        circle(-15 + xM, -120, 3);
        circle(15 + xM, -120, 3);

        glColor3ub(255, 215, 0);
        glBegin(GL_TRIANGLE_STRIP);

        glVertex2f(10 + xM, -105);
        glVertex2f(15 + xM, -115);
        glVertex2f(20 + xM, -102);
        glVertex2f(25 + xM, -112);
        glVertex2f(30 + xM, -105);
        glVertex2f(35 + xM, -115);


        glVertex2f(-10 + xM, -105);
        glVertex2f(-15 + xM, -115);
        glVertex2f(-20 + xM, -102);
        glVertex2f(-25 + xM, -112);
        glVertex2f(-30 + xM, -105);
        glVertex2f(-35 + xM, -115);

        glEnd();

        glColor3ub(255, 0, 0);
        glRectf(-5 + xM, -120, 5 + xM, -90); //jet missile
    }
}

void new_missile() {
    glColor3ub(255, 0, 0);
    glRectf(-5 + xM, -120 + yM, 5 + xM, -90 + yM);
}

void drawplane() { //target

    int randc1, randc2, randc3;

    srand(time(NULL));
    randomY = rand() % 151;
    randc1 = rand() % 256;
    randc2 = rand() % 256;
    randc3 = rand() % 256;

    glColor3ub(randc1, randc2, randc3);
    glRectf(-320 + xP, -60 + randomY, -280 + xP, -40 + randomY);
    glBegin(GL_TRIANGLES);
    glVertex2f(-310 + xP, -60 + randomY);
    glVertex2f(-290 + xP, -60 + randomY);
    glVertex2f(-300 + xP, -70 + randomY);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-310 + xP, -40 + randomY);
    glVertex2f(-290 + xP, -40 + randomY);
    glVertex2f(-300 + xP, -30 + randomY);
    glEnd();
}

void target_hit() { // check for hit

    int mx1 = -5 + xM;
    int mx2 = 5 + xM;
    int my1 = -90 + yM;
    int my2 = -120 + yM;

    int tx1 = -320 + xP;
    int tx2 = -280 + xP;
    int ty1 = -60 + randomY;
    int ty2 = -40 + randomY;

    int points = 0;

    if (mx1 > tx1 && mx2 < tx2)
    {
        if (my1 > ty1 && my2 < ty2)
        {
            yM = 1000;
            xP = 1000;
            score++;
        }
    }
}

void start_Game() {
    glColor3ub(0, 0, 255);
    glRectf(-60, -10, 90, 25); // Start Game Button
    glColor3ub(255, 255, 255);
    vprint(-45, 0, GLUT_BITMAP_HELVETICA_18, "START GAME");
    vprint(180, -120, GLUT_BITMAP_HELVETICA_12, "Homework #2");
    vprint(180, -140, GLUT_BITMAP_HELVETICA_12, "by Rafeh Hussain");
    vprint(-75, -80, GLUT_BITMAP_HELVETICA_12, "Press A to move left");
    vprint(-75, -110, GLUT_BITMAP_HELVETICA_12, "Press D to move right");
}

void run_Game() {
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    vprint(100, -140, GLUT_BITMAP_HELVETICA_18, "Score : %d", score);
    glColor3ub(0, 0, 255);
    glRectf(195, -145, 255, -125);
    glColor3ub(255, 255, 255);
    vprint(200, -140, GLUT_BITMAP_HELVETICA_18, "Finish", score);
    drawJet();
    new_missile();
    drawplane();
    target_hit();
}

void play_again() {

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    vprint(-60, 40, GLUT_BITMAP_HELVETICA_18, "Your Score is %d", score);
    glColor3ub(0, 0, 255);
    glRectf(-60, -10, 80, 25);
    glColor3ub(255, 255, 255);
    vprint(-45, 0, GLUT_BITMAP_HELVETICA_18, "PLAY AGAIN");
}

//
// To display onto window using OpenGL commands
//
void display() {

    // check Game state

    switch (GameState) {
    case OFF: start_Game(); break;
    case RUN: run_Game(); break;
    case OVER: play_again(); break;
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    int i = 0;

    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
    {
        animation = true;
    }


    if (xM > -280)
    {
        if (key == 65 || key == 97) // moving your jet to left
            xM -= 20;
    }

    if (xM < 280)
    {
        if (key == 68 || key == 100) //  moving your jet to right
            xM += 20;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x >= 240 && x <= 390 && y <= 160 && y >= 125) { // Game Start Button
            if (GameState == OFF)
                GameState = RUN;
    }
 
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x >= 495 && x <= 555 && y <= 295 && y >= 275) { // Finish Button
        if (GameState == RUN)
            GameState = OVER;
    }
 
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x >= 240 && x <= 420 && y <= 160 && y >= 125) { // Play Again Button
        if (GameState == OVER)
        {
            score = 0;
            GameState = RUN;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

     glutTimerFunc(timer_period, onTimer, 0);
    // Write your codes here.
    if (animation) //Missile Movement
    {
        if (yM < winHeight - 25)
        {
            yM++;
        }
        else
        {
            yM = 0;
            animation = false;
        }
    }

    if (xP < winWidth - 5) //Target Movement
    {
        xP++;
    }
    else
    {
        xP = 0;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Shoot the Planes Game (by Rafeh Hussain)");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(timer_period, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
