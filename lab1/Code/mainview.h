#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"
#include "cube.h"
#include "pyramid.h"
#include "object.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <memory>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    enum ShadingMode : GLuint {
        PHONG = 0, NORMAL, GOURAUD
    };

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );
    void setUniformLocation();
    void initializeObject();
    void initializeCube();
    void initializePyramid();


private:
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation
    Cube _cube;
    pyramid _pyramid;
    object _object;
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 _rotation_matrix;
    QMatrix4x4 _scaling_matrix;
    QMatrix4x4 _cube_translation_matrix;
    QMatrix4x4 _pyramid_translation_matrix;
    QMatrix4x4 _object_translation_matrix;
    GLint _transformationUniformLocation;
    GLint _projectionUniformLocation;

    void createShaderProgram();
};

#endif // MAINVIEW_H
