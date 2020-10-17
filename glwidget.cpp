#include "glwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>

// Constructor
GLWidget::GLWidget() {
    setWindowTitle("Avaliação 2 - Pirâmide do Minecraft");

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    pyramidHeight = 45;
    cubeSize = 1.0f;

    angleX = posX = 0.0f;
    posY = -pyramidHeight/2;
    posZ = -100 * cubeSize;

    keyMode = 'R';
    stepRotate = 2 * cubeSize;
    stepTranslate = cubeSize;
}

// Destructor
GLWidget::~GLWidget() {
    glDeleteLists(cubeListIndex, 1);
    glDeleteTextures(1, &_textureSand);
}

// Initialize OpenGL
void GLWidget::initializeGL() {
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::black); // Set the clear color to a black background

    glClearDepth(1.0f); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

    glEnable(GL_TEXTURE_2D);
    _textureSand = loadTexture("textures/sand.jpg");

    setupLighting();

    drawCube();
    drawPyramid();
}

GLuint GLWidget::loadTexture(QString fileName) {
    QImage img = convertToGLFormat(QImage(fileName));

    // Texture using linear filter
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    return textureId;
}

void GLWidget::setupLighting() {
    GLfloat ambLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

// This is called when the OpenGL window is resized
void GLWidget::resizeGL(int width, int height) {
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(100.0f, static_cast<GLfloat>(width)/height, 0.1f, 100.0f); // Calculate aspect ratio

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix
}

// OpenGL painting code goes here
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(posX, posY, posZ);
    glRotatef(angleX, 0.0f, 1.0f, 0.0f);

    glCallList(pyramidListIndex);

    // Framerate control
    timer->start(20);
}

void GLWidget::drawCube() {
    cubeListIndex = glGenLists(1);
    glNewList(cubeListIndex, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, _textureSand);
        glBegin(GL_QUADS);
        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-cubeSize, -cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0.0f); glVertex3f( cubeSize, -cubeSize, cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize); glVertex3f( cubeSize, cubeSize, cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, cubeSize); glVertex3f(-cubeSize, cubeSize, cubeSize); // Top Left Of The Texture and Quad
        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(cubeSize, 0.0f); glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize); glVertex3f(-cubeSize, cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, cubeSize); glVertex3f( cubeSize, cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( cubeSize, -cubeSize, -cubeSize); // Bottom Left Of The Texture and Quad
        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, cubeSize); glVertex3f(-cubeSize, cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-cubeSize, cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0.0f); glVertex3f( cubeSize, cubeSize, cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize); glVertex3f( cubeSize, cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(cubeSize, cubeSize); glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, cubeSize); glVertex3f( cubeSize, -cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( cubeSize, -cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0.0f); glVertex3f(-cubeSize, -cubeSize, cubeSize); // Bottom Right Of The Texture and Quad
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(cubeSize, 0.0f); glVertex3f( cubeSize, -cubeSize, -cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize); glVertex3f( cubeSize, cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, cubeSize); glVertex3f( cubeSize, cubeSize, cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f( cubeSize, -cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0.0f); glVertex3f(-cubeSize, -cubeSize, cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize); glVertex3f(-cubeSize, cubeSize, cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, cubeSize); glVertex3f(-cubeSize, cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glEnd();
    glEndList();
}

void GLWidget::drawPyramid() {
    pyramidListIndex = glGenLists(1);
    glNewList(pyramidListIndex, GL_COMPILE);
        GLuint size;
        GLuint offset = 2 * cubeSize;

        // A primeira linha é considerada o topo da pirâmide
        for(GLuint row = 0; row < pyramidHeight; row++) {
            size = 1 + (row * 2); // Número de elementos em cada eixo

            glPushMatrix();
                glTranslatef(0.0f, (pyramidHeight - row) * offset, 0.0f); // Desce uma linha da pirâmide
                glTranslatef(-(row * offset), 0.0f, 0.0f); // Posiciona a origem no ponto mais extremo de X

                for(GLuint x = 0; x < size; x++) {
                    glPushMatrix();
                        glTranslatef(x * offset, 0.0f, 0.0f); // Move a origem para o próximo ponto de X
                        glTranslatef(0.0f, 0.0f, -(row * offset)); // Posiciona a origem no ponto mais extremo de Z

                        for(GLuint z = 0; z < size; z++) {
                            glTranslatef(0.0f, 0.0f, offset);  // Move a origem para o próximo ponto de Z
                            glCallList(cubeListIndex);
                        }
                    glPopMatrix();
                }
            glPopMatrix();
        }

    glEndList();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;

        case Qt::Key_R: // Altera o modo para rotação
            keyMode = 'R';
            break;

        case Qt::Key_T: // Altera o modo para translação
            keyMode = 'T';
            break;

        case Qt::Key_Up:
            if(keyMode == 'T') { // Translação
                posZ -= stepTranslate;
            }
            break;

        case Qt::Key_Down:
            if(keyMode == 'T') { // Translação
                posZ += stepTranslate;
            }
            break;

        case Qt::Key_Left:
            if(keyMode == 'R') { // Rotação
                angleX -= stepRotate;
                angleX = angleX < 0 ? angleX + 360 : angleX;
            } else if(keyMode == 'T') { // Translação
                posX -= stepTranslate;
            }
            break;

        case Qt::Key_Right:
            if(keyMode == 'R') { // Rotação
                angleX += stepRotate;
                angleX = angleX > 360 ? angleX - 360 : angleX;
            } else if(keyMode == 'T') { // Translação
                posX += stepTranslate;
            }
            break;

        default:
            QGLWidget::keyPressEvent(event);
    }
}

void GLWidget::changeEvent(QEvent *event) {
    switch (event->type()) {
        default:
            break;
    }
}
