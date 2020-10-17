#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QHash>

class QTimer;

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget();
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

private:
    void setupLighting();
    GLuint loadTexture(QString image);
    void drawCube();
    void drawPyramid();

    GLuint _textureSand;
    GLuint cubeListIndex, pyramidListIndex;
    QTimer *timer;

    GLuint pyramidHeight;
    GLfloat cubeSize;

    GLchar keyMode;
    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, posX, posY, posZ;
};

#endif // GLWIDGET_H
