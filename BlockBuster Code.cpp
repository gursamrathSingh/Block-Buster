#include <GL/glut.h>
#include <vector>
#include <ctime>
#include <string>
#include <cmath>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;
const float PLAYER_WIDTH = 50.0f;
const float PLAYER_HEIGHT = 20.0f;
const float BULLET_RADIUS = 5.0f;
const float ENEMY_SIZE = 30.0f;
const float TROJAN_SIZE = 40.0f;

float playerX = WIDTH / 2.0f;
float bulletSpeed = 5.0f;
float enemySpeed = 1.0f;
int killCount = 0;

bool leftPressed = false, rightPressed = false;

struct Bullet {
    float x, y;
    bool alive;
};

struct Enemy {
    float x, y;
    bool alive;
    bool isTrojan;
    int hitCount;  // To count hits for Trojan enemies
    float blinkTimer;  // For controlling the blinking effect
};

struct Explosion {
    float x, y;
    int timer;
};

std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Explosion> explosions;

void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float x, float y, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159f / 180;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

void drawExplosion(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Outer flare 
    glColor3f(1.0, 0.3, 0.0);
    drawCircle(0, 0, 18);

    // Mid flare
    glColor3f(1.0, 1.0, 0.0);
    drawCircle(0, 0, 10);

    // Inner core 
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(0, 0, 5);

    // Sparks
    glColor3f(1.0, 0.7, 0.1);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; ++i) {
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float dist = rand() % 10 + 5;
        glVertex2f(cos(angle) * dist, sin(angle) * dist);
    }
    glEnd();

    glPopMatrix();
}

void drawPlayer() {
    float centerX = playerX + PLAYER_WIDTH / 2;
    float y = 20;

    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(centerX - 5, y, 10, 40);

    glColor3f(0.0f, 1.0f, 1.0f);
    drawRect(centerX - 3, y + 20, 6, 10);

    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(centerX - 5, y + 15);
    glVertex2f(centerX - 20, y + 5);
    glVertex2f(centerX - 5, y);

    glVertex2f(centerX + 5, y + 15);
    glVertex2f(centerX + 20, y + 5);
    glVertex2f(centerX + 5, y);
    glEnd();
}

void drawEnemy(float x, float y, bool isTrojan, float blinkTimer) {
    float cx = x + (isTrojan ? TROJAN_SIZE : ENEMY_SIZE) / 2;
    float cy = y;

    if (isTrojan) {
        // Trojans: Bigger and flashing
        glColor3f(1.0f, 0.0f, 1.0f); 
        if (int(blinkTimer) % 2 == 0)
            glColor3f(1.0f, 0.0f, 1.0f); // Flashing color 
        else
            glColor3f(0.6f, 0.0f, 0.6f); 
        drawRect(cx - 15, cy, 30, 40); // Trojan shape
    }
    else {
        // Normal Enemies: Smaller and steady color
        glColor3f(1.0f, 0.0f, 0.0f); 
        drawRect(cx - 10, cy, 20, 30); // Normal shape
    }
}

void drawLegend() {
    float boxX = WIDTH - 220;
    float boxY = HEIGHT - 260; 
    float boxW = 200;
    float boxH = 250; 

    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(boxX, boxY, boxW, boxH);

    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(boxX, boxY);
    glVertex2f(boxX + boxW, boxY);
    glVertex2f(boxX + boxW, boxY + boxH);
    glVertex2f(boxX, boxY + boxH);
    glEnd();

    glColor3f(1,1,1);
    drawText(boxX + 10, boxY + 235, " ~ Computer Graphics Project ~ ");
    drawText(boxX + 10, boxY + 220, "      ~~~ Block Buster ~~~ ");

    glColor3f(1, 1, 1);
    drawText(boxX + 10, boxY + 185, "LEFT/RIGHT : Move ship");
    drawText(boxX + 10, boxY + 170, "SPACE BAR : Shoot bullet");
    drawText(boxX + 10, boxY + 155, "ESC : To exit");

    drawText(boxX + 10, boxY + 130, "+1 Point for Normal Enemy");
    drawText(boxX + 10, boxY + 115, "+3 Points for Trojan Enemy");
    drawText(boxX + 10, boxY + 85, "Trojan Enemy: Purple");
    drawText(boxX + 10, boxY + 70, "Normal Enemy: Red");

    

    glColor3f(1,1,1);
    drawText(boxX + 10, boxY + 35, "Contributors -");
    drawText(boxX + 10, boxY + 20, "Gursamrath Singh(102217202)");
    drawText(boxX + 10, boxY + 5, "Jatin (102217190)");
    
}

void spawnEnemy() {
    Enemy e;
    e.x = float(rand() % (WIDTH - int(ENEMY_SIZE)));
    e.y = HEIGHT;
    e.alive = true;
    e.isTrojan = rand() % 2 == 0;
    e.hitCount = 0;  // Initial hits count
    e.blinkTimer = 0.0f; // Initially not blinking
    enemies.push_back(e);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPlayer();

    glColor3f(1, 1, 0);
    for (auto& b : bullets)
        if (b.alive) drawCircle(b.x, b.y, BULLET_RADIUS);

    for (auto& e : enemies)
        if (e.alive) drawEnemy(e.x, e.y, e.isTrojan, e.blinkTimer);

    for (auto& ex : explosions)
        if (ex.timer > 0) drawExplosion(ex.x, ex.y);

    glColor3f(1, 1, 1);
    drawText(10, HEIGHT - 30, "Kills: " + std::to_string(killCount), GLUT_BITMAP_HELVETICA_18);

    drawLegend();  

    glutSwapBuffers();
}

void update(int value) {
    if (leftPressed && playerX > 0) playerX -= 3;
    if (rightPressed && playerX < WIDTH - PLAYER_WIDTH) playerX += 3;

    for (auto& b : bullets) {
        if (b.alive) b.y += bulletSpeed;
        if (b.y > HEIGHT) b.alive = false;
    }

    for (auto& e : enemies) {
        if (e.alive) e.y -= enemySpeed;
        if (e.y < 0) e.alive = false;
    }

    // Checking for collisions between bullets and enemies
    for (auto& b : bullets) {
        for (auto& e : enemies) {
            if (b.alive && e.alive && b.x > e.x && b.x < e.x + (e.isTrojan ? TROJAN_SIZE : ENEMY_SIZE) &&
                b.y > e.y && b.y < e.y + (e.isTrojan ? TROJAN_SIZE : ENEMY_SIZE)) {
                b.alive = false;
                if (e.isTrojan) {
                    e.hitCount++; // Increase hit count for Trojan
                    if (e.hitCount >= 2) {
                        e.alive = false;
                        killCount += 3; // +3 points for Trojan
                        explosions.push_back({ e.x + (e.isTrojan ? TROJAN_SIZE : ENEMY_SIZE) / 2, e.y + (e.isTrojan ? TROJAN_SIZE : ENEMY_SIZE) / 2, 15 });
                    }
                }
                else {
                    e.alive = false;
                    killCount += 1; // +1 point for regular enemy
                    explosions.push_back({ e.x + ENEMY_SIZE / 2, e.y + ENEMY_SIZE / 2, 15 });
                }
            }
        }
    }

    // Update Trojan blink timer
    for (auto& e : enemies) {
        if (e.isTrojan && e.hitCount > 0) {
            e.blinkTimer += 0.1f;
            if (e.blinkTimer > 2.0f) e.blinkTimer = 0.0f; // Reset blink timer
        }
    }

    // Remove explosions after they have been shown for a brief period
    for (auto it = explosions.begin(); it != explosions.end();) {
        if (--it->timer <= 0)
            it = explosions.erase(it); // Remove explosion after timer reaches 0
        else
            ++it;
    }

    if (rand() % 100 < 2) spawnEnemy();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void specialDown(int key, int, int) {
    if (key == GLUT_KEY_LEFT) leftPressed = true;
    if (key == GLUT_KEY_RIGHT) rightPressed = true;
}

void specialUp(int key, int, int) {
    if (key == GLUT_KEY_LEFT) leftPressed = false;
    if (key == GLUT_KEY_RIGHT) rightPressed = false;
}

void keyPressed(unsigned char key, int, int) {
    if (key == ' ') {
        bullets.push_back({ playerX + PLAYER_WIDTH / 2, 40, true });
    }

    if (key == 27)
        exit(0);
}

int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Block - Buster");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f);

    glutDisplayFunc(display);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyPressed);
    glutTimerFunc(25, update, 0);

    glutMainLoop();

    return 0;
}