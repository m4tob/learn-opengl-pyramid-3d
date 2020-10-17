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
    GLuint loadTexture(QString image);
    void drawCube();

    char keyMode;

    GLfloat angleX, angleY, posX, posY;
    GLfloat stepRotate, stepTranslate;
    GLfloat z;

    GLuint _textureSand;

    QTimer *timer;
    GLuint filter, texture[3];
    GLfloat xrot, yrot, xspeed, yspeed;
    bool lightChanged, filterChanged;
    int changeAmount;
    QHash<GLuint, QString> filterName;
};

#endif // GLWIDGET_H
