#include <GL/glut.h>
#include <cmath>
#include <ctime>

using namespace std;
float angleMercury = 0.0, angleVenus = 0.0, angleEarth = 0.0, angleMars = 0.0;
float distanceMercury = 6.0, distanceVenus = 10.0, distanceEarth = 14.0, distanceMars = 18.0;
float zoomFactor = 1.0;
bool showOrbitPaths = true;
bool rotateView = false;
int lastMouseX = 0, lastMouseY = 0;
bool cometShowerActive = false;
float cometX = 0.0, cometY = 0.0, cometZ = 0.0;
int ar[1000][3];
bool stop  = false;



void drawOrbitPath(float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i) {
        float angle =  float(i) * 3.14159265f / 180.0f;
        glVertex3f(radius * cos(angle), 0.0, radius * sin(angle));
    }
    glEnd();
}

void drawPlanet(float distance, float angle, float radius) {
        glPushMatrix();

    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(distance * zoomFactor, 0.0, 0.0);


    if (distance == distanceMercury) {
        glColor3f(0.6, 0.6, 0.6); 
    } else if (distance == distanceVenus) {
        glColor3f(0.8, 0.5, 0.2); 
    } else if (distance == distanceEarth) {
        glColor3f(0.0, 0.4, 0.8); 
    } else if (distance == distanceMars) {
        glColor3f(0.8, 0.2, 0.0);
    }

    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

void drawStars() {
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 1000; ++i) {
        glVertex3f(ar[i][0], ar[i][1], ar[i][2]);
    }
    glEnd();
}

void calculateCometPath(float time, float& x, float& y, float& z) {
    x = 5.0 * cos(0.5 * time);
    y = 2.0 * sin(0.8 * time);
    z = 5.0 * sin(0.5 * time);
}

void drawComet() {
    float cometTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;  

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(cometX, cometY, cometZ);

    float newX, newY, newZ;
    calculateCometPath(cometTime, newX, newY, newZ);

    glVertex3f(newX, newY, newZ);
    glEnd();

    cometX = newX;
    cometY = newY;
    cometZ = newZ;
}


void display() {
    if (stop) return ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawStars();

    if (showOrbitPaths) {
        glColor3f(0.5, 0.5, 0.5);
        drawOrbitPath(distanceMercury);
        drawOrbitPath(distanceVenus);
        drawOrbitPath(distanceEarth);
        drawOrbitPath(distanceMars);
    }

    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(2.0, 20, 20);

    drawPlanet(distanceMercury, angleMercury, 0.3);
    drawPlanet(distanceVenus, angleVenus, 0.5);
    drawPlanet(distanceEarth, angleEarth, 0.5);
    drawPlanet(distanceMars, angleMars, 0.4);


    if (cometShowerActive) {
        drawComet();
        cometX += 0.2;
        cometY += 0.1;
        cometZ += 0.15;

        if (cometX > 20.0) {
            cometShowerActive = false;
            cometX = 0.0;
            cometY = 0.0;
            cometZ = 0.0;
        }
    }
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void update(int value) {
    angleMercury += 2.0;
    angleVenus += 1.5;
    angleEarth += 1.0;
    angleMars += 0.8;

    if (angleMercury > 360) angleMercury -= 360;
    if (angleVenus > 360) angleVenus -= 360;
    if (angleEarth > 360) angleEarth -= 360;
    if (angleMars > 360) angleMars -= 360;

    if (rand() % 30 == 0) {
        cometShowerActive = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            zoomFactor -= 0.1;
            break;
        case GLUT_KEY_DOWN:
            zoomFactor += 1;
            break;
        case GLUT_KEY_LEFT:
            angleMercury -= 5.0;
            angleVenus -= 5.0;
            angleEarth -= 5.0;
            angleMars -= 5.0;
            break;
        case GLUT_KEY_RIGHT:
            angleMercury += 5.0;
            angleVenus += 5.0;
            angleEarth += 5.0;
            angleMars += 5.0;
            break;
    }

    if (zoomFactor < 0.1) zoomFactor = 0.1;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 32:
            stop = not stop;
            break;
        case 'o':
        case 'O':
            showOrbitPaths = !showOrbitPaths;
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}




int main(int argc, char** argv) {

for (int i = 0; i < 1000; ++i) {
        ar[i][0] = rand() % 200 - 100;
        ar[i][1]= rand() % 200 - 100;
        ar[i][2] = rand() % 200 - 100;
    }
    srand((time(0)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Solar System Simulation");
    glutInitWindowSize(800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
