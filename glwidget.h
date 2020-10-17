/*
 * UNIVERSIDADE ESTADUAL DE FEIRA DE SANTANA - UEFS
 * Engenharia da Computação
 * TEC431 - Computação Gráfica
 *
 * 2ª Avaliação - 2019.2E
 *
 * Alunos:
 *      Matheus Oliveira Borges <matob@live.com>
 *      Luiz Ricardo Inês de Souza <lzricardo.ecomp@gmail.com>
 *
 * -------------------------------------------------------------------------------
 *
 * ============================================================================
 * ===============================  INSTRUÇÕES  ===============================
 * ============================================================================
 *
 * Para rotacionar é necessário pressionar a tecla R (ativando o modo de rotação)
 * e em seguida, utilizar as teclas direcionais.
 *
 * Para transladar, pressione a tecla T (ativando o modo de translação) e, também
 * utilizando as teclas direcionais, pode-se mover o modelo.
 *
 */

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
    void setupTextures();
    GLuint loadTexture(QString image);
    void setupLighting();
    void paintSky();
    void drawGrass();
    void drawCube();
    void drawPyramid();

    GLuint _textureGrass, _textureSand;
    GLuint grassListIndex, cubeListIndex, pyramidListIndex;
    QTimer *timer;

    GLint pyramidHeight;
    GLfloat cubeSize;

    GLchar keyMode;
    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, angleY, posX, posY, posZ;
};

#endif // GLWIDGET_H
