//  Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <string.h>
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawRectPolygon(int x, int y, int w, int h);
void drawRectQuad(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void player();
void border(int x, int y);
void drawStar(float x, float y, float radius, float height, float rotationAngle);
void drawTriangle(float x, float y, float width, float height, float angle);
void drawBorders();
void drawHeart(float xOffset, float yOffset, float scale);
void drawDeadHeart(float xOffset, float yOffset, float scale);
void drawObstacles(float x, float y);
void drawObstacles1(float x, float y);
void drawObstacles2(float x, float y);
void drawCollectibile(float x, float y);
void drawSpikes(float x, float y);
void drawPowerUps(float x, float y);
void renderBitmapText(float x, float y, void* font, const char* text);
void drawArrowDown(float x, float y, float width, float height);
void drawHollowCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void drawPoint(float x, float y);
void Update(int value);
void Display();


//-----------------


int borderSizeX = 800; //border length
int borderSizeY = 100;   //border height
int borderOffset = 10;
int playerPositionX = 10;
int playerPositionY = 0;
int playerOffset = borderSizeY;
int isDucking = 0;
int isJumping = 0;
float jumpHeight = 100;
float difficultyMultiplier = 1;
int projectileSpeed = 10; //in ms
float SpawnPointsX = 1000; 
float SpawnPointsY = 150;//will be random
float obstacleSpawnPointsX = SpawnPointsX;
float obstacleSpawnPointsY = SpawnPointsY;
float obstacle2SpawnPointsX = SpawnPointsX;
float obstacle2SpawnPointsY = SpawnPointsY;
float obstacle3SpawnPointsX = SpawnPointsX;
float obstacle3SpawnPointsY = SpawnPointsY;
float collectibleSpawnPointsX = SpawnPointsX;
float collectibleSpawnPointsY = SpawnPointsY;
float powerSpawnPointsX = SpawnPointsX;
float powerSpawnPointsY = SpawnPointsY;
float power2SpawnPointsX = SpawnPointsX;
float power2SpawnPointsY = SpawnPointsY;
float jumpVelocity = 20;   
float velocityY = 0;
int gameLoose = 0;
float groundLevel = 0;
int isgrounded = 1;
int score = 0;
int health = 5;
float gameTime = 10; // in seconds
float playerY = 0;
float playerX = 0;
float scaleX = 50;
float scaleY = 150;
float originalY = playerPositionY;
int activeObstacle = 0; 
int spawnDelay = 1000;
float gravity = -0.25f;   
float jumpForce = 10.0f; 
int invincibility = 0;
int doubleScore = 1;
int activePower = 0;
int collectibileIdle = 1;
int sunY = 600;
int sun = 0;
int coin = 0;
//-----------------



void renderBitmapText(float x, float y, void* font, const char* text) {
    glRasterPos2f(x, y);  // Set the position to start drawing text

    // Loop through each character in the string and display it
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    // AABB (Axis-Aligned Bounding Box) collision detection
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void resetObstacle(float& obstacleX, float& obstacleY) {
    obstacleX = SpawnPointsX;
    obstacleY = rand() % (200 - 100 + 1) + 120;  
}


void player() {
    glColor3f(0, 0, 0);
    glPushMatrix();

    if (isDucking& isgrounded) {
        glScaled(1, 0.5, 1);
        drawRectPolygon(10, playerPositionY+ borderSizeY + playerOffset, 20, 20);//legs
        drawRectQuad(40, playerPositionY + borderSizeY + playerOffset, 20, 20);//legs
        drawRect(10, playerPositionY + borderSizeY + playerOffset + 20, 50, 100);//body
        drawCircle(35, playerPositionY + borderSizeY + playerOffset + 80, 20); //face
        playerY = playerPositionY + borderSizeY + playerOffset;
        playerX = 10;
        scaleX = 50;
        scaleY = 120;


    }
    else {
        glScaled(1, 1, 1);
        drawRectPolygon(10, playerPositionY + borderSizeY, 20, 20);//legs
        drawRectQuad(40, playerPositionY + borderSizeY, 20, 20);//legs
        drawRect(10, playerPositionY + borderSizeY + 20, 50, 100);//body
        drawCircle(35, playerPositionY + borderSizeY + 80, 20); //face
        playerY = playerPositionY + borderSizeY ;
        playerX = 10;
        scaleX = 50;
        scaleY = 120;


    }
    

    

    glPopMatrix();

}

void drawPoint(float x, float y) {
    glColor3f(1, 0, 0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);  
    glVertex2f(x,y);  
    glEnd();

}


void drawCollectibile(float x, float y) {

    glColor3f(1,0.5,0);
    drawHollowCircle(x, y, 10*coin);
    drawCircle(x, y, 7);

}


void drawPowerUps2(float x, float y) {
    glColor3f(1, 1, 0);
    drawArrowDown(x, y, 20, 30);

}

void drawPowerUps(float x, float y) {
    glColor3f(0, 1, 0);
    drawArrowDown(x, y, 20, 30);

}

void drawArrowDown(float x, float y, float width, float height) {
    float arrowheadHeight = height * 0.4f;  // 40% of the arrow's height is for the arrowhead
    float shaftHeight = height - arrowheadHeight;  // Remaining height for the shaft

    // Draw the shaft of the arrow (rectangle)
    glBegin(GL_QUADS);  // Draw a rectangle for the arrow's shaft
    glVertex2f(x - width / 4, y);  // Bottom-left
    glVertex2f(x + width / 4, y);  // Bottom-right
    glVertex2f(x + width / 4, y + shaftHeight);  // Top-right
    glVertex2f(x - width / 4, y + shaftHeight);  // Top-left
    glEnd();

    // Draw the arrowhead (triangle)
    glBegin(GL_TRIANGLES);  // Draw a triangle for the arrowhead
    glVertex2f(x, y + shaftHeight + arrowheadHeight);  // Top vertex of the triangle (arrowhead)
    glVertex2f(x - width / 2, y + shaftHeight);  // Bottom-left of the triangle
    glVertex2f(x + width / 2, y + shaftHeight);  // Bottom-right of the triangle
    glEnd();
}


void drawHeart(float xOffset, float yOffset, float scale) {
    glBegin(GL_POLYGON); // Begin drawing the heart as a polygon

    // Loop through t values to draw the heart using parametric equations
    for (float t = 0; t <= 2 * 3.14159f; t += 0.01f) {
        // Parametric equations for the heart shape
        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        // Apply scaling and offsets to position and size the heart
        glVertex2f(xOffset + x * scale, yOffset + y * scale);
    }

    glEnd();
}
void drawDeadHeart(float xOffset, float yOffset, float scale) {
    glBegin(GL_POINTS); // Begin drawing the heart as a polygon

    // Loop through t values to draw the heart using parametric equations
    for (float t = 0; t <= 2 * 3.14159f; t += 0.01f) {
        // Parametric equations for the heart shape
        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        // Apply scaling and offsets to position and size the heart
        glVertex2f(xOffset + x * scale, yOffset + y * scale);
    }

    glEnd();
}


void drawObstacles(float x, float y ) {
    glColor3f(1,0 ,0 );
    drawRectPolygon(x, y, 20, 20);
    drawTriangle(x - 20, y , 20, 20,90);
}
void drawObstacles1(float x, float y) {
    glColor3f(1, 0, 0);
    drawRectPolygon(x, y, 20, 20);
}
void drawObstacles2(float x, float y) {
    glColor3f(1, 0, 0);
    drawTriangle(x - 20, y, 20, 20, 90);
}
void drawSpikes(float x, float y) {
    glColor3f(1, 0, 0);
    drawTriangle(x - 20, y, 20, 20, 0);
}



void border(int x, int y) {
    drawRect(x, y, borderSizeX, borderSizeY); // lower border
    glColor3f(1, 0, 1);
    drawRect(x + borderOffset, y + borderOffset, borderSizeX - 20, borderSizeY - 20);
}

void drawBorders() {
    glColor3f(1, 0, 0);
    border(0, 0);
    glColor3f(1, 0, 0);
    border(0, borderSizeX - borderSizeY);
}



void main(int argc, char** argr) {
    glutInit(&argc, argr);

    glutInitWindowSize(1000, 560);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Infinite Runner");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
    glutKeyboardUpFunc(KeyUp);      // sets the Keyboard handler function; called when a key is pressed
    glutTimerFunc(0, Update, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}

void drawTriangle(float x, float y, float width, float height, float angle) {
    glPushMatrix();
    glTranslatef(x + width / 2.0f, y + height / 2.0f, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-(x + width / 2.0f), -(y + height / 2.0f), 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width / 2.0f, y + height);
    glEnd();  
    glPopMatrix();
}


void drawRect(int x, int y, int w, int h) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glVertex2f(x, y);
    glEnd();
}

void drawRectPolygon(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glVertex2f(x, y);
    glEnd();
}
void drawRectQuad(int x, int y, int w, int h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glVertex2f(x, y);
    glEnd();
}

void drawHollowCircle(int x, int y, float r) {
    int numSegments = 100;  // Number of segments to approximate the circle
    float angle;

    glBegin(GL_POINTS);  // Start drawing the circle as an outline

    // Loop to generate points around the circumference
    for (int i = 0; i < numSegments; i++) {
        angle = 2.0f * 3.14159f * i / numSegments;  // Calculate the angle for each segment

        float dx = r * cosf(angle);  // X coordinate
        float dy = r * sinf(angle);  // Y coordinate

        glVertex2f(x + dx, y + dy);  // Specify the vertex
    }

    glEnd();  // End drawing the circle
}


void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

void resetDucking(int value) {
    isDucking = 0;
}

void resetScore(int value) {
    doubleScore = 1;
}
void resetInvincibility(int value) {
    invincibility = 0;
}
void KeyUp(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        isDucking = 0;  // Reset ducking state when the key is released
    }
    // Handle other key up events...
}

// Keyboard handler function
void Key(unsigned char key, int x, int y) {

    // Ducking logic
    if (key == 's' && !isJumping) {
        isDucking = 1;
        
    }
    // Ducking logic
    if (key == 's' && isJumping &&!isgrounded) {
        playerPositionY = groundLevel;  // Snap the player to the ground
        isJumping = 0;  // Reset the jumping state
        isgrounded = 1;  // Player is now on the ground
        velocityY = 0;  // Reset vertical velocity
        isDucking = 1;
        
    }


    // Jump logic (pressing 'w' to jump)
    if (key == 'w' && isgrounded && !isJumping) {  // Only jump if on the ground and not already jumping
        isJumping = 1;               // Set jumping state
        velocityY = jumpForce;        // Apply jump force to the velocity
        isgrounded = 0;               // Player is no longer on the ground
        isDucking = 0;
    }



    // Ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}



void resetCoin() {
    coin = 0;

}
void setCoin() {
    coin = 1;
    
}


void Update(int value) {

    float yDiff = fabs(obstacleSpawnPointsY - collectibleSpawnPointsY);
    float yDiff2 = fabs(obstacle2SpawnPointsY - collectibleSpawnPointsY);
    float yDiff3 = fabs(obstacle3SpawnPointsY - collectibleSpawnPointsY);
    float pDiff = fabs(obstacleSpawnPointsX - powerSpawnPointsX);
    float pDiff2 = fabs(obstacle2SpawnPointsX - powerSpawnPointsX);
    float pDiff3 = fabs(obstacle3SpawnPointsX - powerSpawnPointsX);
    float p2Diff = fabs(obstacleSpawnPointsX - power2SpawnPointsX);
    float p2Diff2 = fabs(obstacle2SpawnPointsX - power2SpawnPointsX);
    float p2Diff3 = fabs(obstacle3SpawnPointsX - power2SpawnPointsX);
    float cDiff = fabs(collectibleSpawnPointsX - powerSpawnPointsX);
    float cDiff2 = fabs(collectibleSpawnPointsX - power2SpawnPointsX);

    int randomSpawn = rand() % 2;
    

    if ((int)gameTime%2==0) {
        setCoin();
    }
    else {
        resetCoin();
    }



    if (!isgrounded) {
        velocityY += gravity;  
        playerPositionY += velocityY;  

        
        if (playerPositionY <= groundLevel) {
            playerPositionY = groundLevel;  
            isJumping = 0;  
            isgrounded = 1;  
            velocityY = 0;  
        }
    }

    


    if (activeObstacle == 0) {
        activeObstacle = rand() % 3 + 1;  
    }

    if (activeObstacle == 1) {
        
        obstacleSpawnPointsX -= 1 * difficultyMultiplier;
        if (pDiff < 30) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
        }
        if (p2Diff<30) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
        }
        if (obstacleSpawnPointsX < 0) {
            resetObstacle(obstacleSpawnPointsX, obstacleSpawnPointsY);
            activeObstacle = 0;
        }
    }
    else if (activeObstacle == 2) {
        obstacle2SpawnPointsX -= 1 * difficultyMultiplier;
        if (pDiff2 < 30) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
        }
        if (p2Diff2 < 30) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
        }
        if (obstacle2SpawnPointsX < 0) {
            resetObstacle(obstacle2SpawnPointsX, obstacle2SpawnPointsY);
            activeObstacle = 0;
        }
    }
    else if (activeObstacle == 3) {
        obstacle3SpawnPointsX -= 1 * difficultyMultiplier;
        if (pDiff3 < 30) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
        }
        if (p2Diff3 < 30) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
        }
        if (obstacle3SpawnPointsX < 0) {
            resetObstacle(obstacle3SpawnPointsX, obstacle3SpawnPointsY);
            activeObstacle = 0;
        }
    }

    collectibleSpawnPointsX -= 1 * difficultyMultiplier;
    if (collectibleSpawnPointsX < 0) {
        resetObstacle(collectibleSpawnPointsX, collectibleSpawnPointsY);
    }

    if (yDiff < 30 || yDiff2 < 30 || yDiff3 < 30 && (cDiff < 30 || cDiff2 < 30)) {
        resetObstacle(collectibleSpawnPointsX, collectibleSpawnPointsY);
    }


    

    if (activePower == 0) {
        activePower = rand() % 2 + 1;
    }

    if (activePower==1) {
        powerSpawnPointsX -= 1 * difficultyMultiplier;
        if (powerSpawnPointsX < 0) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
            activePower = 0;
        }
    }
    else {
        power2SpawnPointsX -= 1 * difficultyMultiplier;
        if (power2SpawnPointsX < 0) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
            activePower = 0;
        }
    }
    
    if (powerSpawnPointsY >= 200  ) {
        collectibileIdle = 0;

    }
    if (powerSpawnPointsY <= 120) {
        collectibileIdle = 1;
    }
    
    
    
    if (collectibileIdle) {
        powerSpawnPointsY += 1;
    }
    else {
        powerSpawnPointsY -= 1;
    }


    if (sunY >= 600) {
        sun = 0;

    }
    if (sunY <= 550) {
        sun = 1;
    }

    if (sun) {
        sunY += 1;
    }
    else {
        sunY -= 1;
    }



    if (power2SpawnPointsY >= 200) {
        collectibileIdle = 0;

    }
    if (power2SpawnPointsY <= 120) {
        collectibileIdle = 1;
    }



    if (collectibileIdle) {
        power2SpawnPointsY += 1;
        
    }
    else {
        power2SpawnPointsY -= 1;
        
    }

    
    difficultyMultiplier += 0.005;


    gameTime -= 0.01;  


    if (gameTime <= 0) {
        gameLoose = 1;
        gameTime = 0;  
    }


    
    



    if (!isDucking) {
        
        if (checkCollision(playerX, playerY, scaleX, scaleY, obstacleSpawnPointsX, obstacleSpawnPointsY, 20, 20)) {
           
            obstacleSpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY, scaleX, scaleY, obstacle2SpawnPointsX, obstacle2SpawnPointsY, 20, 20)) {

            obstacle2SpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY, scaleX, scaleY, obstacle3SpawnPointsX, obstacle3SpawnPointsY, 20, 20)) {
            
            obstacle3SpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY, scaleX, scaleY, collectibleSpawnPointsX-10, collectibleSpawnPointsY, 20, 20)) {
            resetObstacle(collectibleSpawnPointsX, collectibleSpawnPointsY);
            score += 50 * doubleScore;
        }
        if (checkCollision(playerX, playerY, scaleX, scaleY, powerSpawnPointsX-10, powerSpawnPointsY, 20, 30)) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
            doubleScore = 2;
            glutTimerFunc(5000, resetScore, 0);
        }
        if (checkCollision(playerX, playerY, scaleX, scaleY, power2SpawnPointsX, power2SpawnPointsY, 20, 20)) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
            invincibility = 1;
            glutTimerFunc(5000, resetInvincibility, 0);
        }
    }
    else {
        
        if (checkCollision(playerX, playerY-playerOffset, scaleX, scaleY-60, obstacleSpawnPointsX-20, obstacleSpawnPointsY, 40, 20)) {
            
            obstacleSpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY - playerOffset, scaleX, scaleY - 60, obstacle2SpawnPointsX, obstacle2SpawnPointsY, 20, 20)) {
            
            obstacle2SpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY - playerOffset, scaleX, scaleY - 60, obstacle3SpawnPointsX-20, obstacle3SpawnPointsY, 20, 20)) {
            
            obstacle3SpawnPointsX += 300;
            difficultyMultiplier = 1;
            if (invincibility == 0) {
                health -= 1;
            }
        }
        if (checkCollision(playerX, playerY - playerOffset, scaleX, scaleY - 60, collectibleSpawnPointsX-10, collectibleSpawnPointsY, 20, 20)) {
            resetObstacle(collectibleSpawnPointsX, collectibleSpawnPointsY);
            score += 50 * doubleScore;
        }
        if (checkCollision(playerX, playerY - playerOffset, scaleX, scaleY - 60, powerSpawnPointsX-10, powerSpawnPointsY, 20, 30)) {
            resetObstacle(powerSpawnPointsX, powerSpawnPointsY);
            doubleScore = 2;
            glutTimerFunc(5000, resetScore, 0);
        }
        if (checkCollision(playerX, playerY - playerOffset, scaleX, scaleY - 60, power2SpawnPointsX, power2SpawnPointsY, 20, 20)) {
            resetObstacle(power2SpawnPointsX, power2SpawnPointsY);
            invincibility = 1;
            glutTimerFunc(5000, resetInvincibility, 0);
        }
    }
    

    
    glutPostRedisplay();  


    glutTimerFunc(projectileSpeed, Update, 0);
}




void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBorders();

    if (!gameLoose && health > 0) {

        char ScoreBuffer[50];
        char TimeBuffer[50];
        sprintf(ScoreBuffer, "Score: %d", (int)score);
        sprintf(TimeBuffer, "Time: %d", (int)gameTime);
        renderBitmapText(30, 750, GLUT_BITMAP_HELVETICA_18, ScoreBuffer);
        renderBitmapText(30, 720, GLUT_BITMAP_HELVETICA_18, TimeBuffer);


        for (int i = 0; i < 5; i++) drawDeadHeart(600 + (i * 40), 740, 1);
        for (int i = 0; i < health; i++) drawHeart(600 + (i * 40), 740, 1);

        


        player();


        glColor3f(1, 0.651, 0);
        drawCircle(700, sunY, 50);
        drawObstacles(obstacleSpawnPointsX, obstacleSpawnPointsY);
        drawObstacles1(obstacle2SpawnPointsX, obstacle2SpawnPointsY);
        drawObstacles2(obstacle3SpawnPointsX, obstacle3SpawnPointsY);
        drawCollectibile(collectibleSpawnPointsX, collectibleSpawnPointsY);
        drawPowerUps(powerSpawnPointsX, powerSpawnPointsY);
        drawPowerUps2(power2SpawnPointsX, power2SpawnPointsY);

    }
    else {

        renderBitmapText(30, 750, GLUT_BITMAP_HELVETICA_18, "You Lost!");
    }


    
    if ((int)gameTime <= 0) {
        gameLoose = 1;
    }


    glFlush();
}
