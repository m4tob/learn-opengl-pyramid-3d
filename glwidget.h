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
    void setupLight();
    GLuint loadTexture(QString image);
    void drawPyramid();
    void drawCube();

    int pyramidHeight;
    GLfloat cubeSize;

    char keyMode;

    GLfloat angleX, posX, posY, posZ;
    GLfloat stepRotate, stepTranslate;

    GLuint cubeListIndex;
    GLuint _textureSand;

    QTimer *timer;
    QHash<GLuint, QString> filterName;
};

#endif // GLWIDGET_H
