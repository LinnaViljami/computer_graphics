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
#include <phongshader.h>
#include <normalshader.h>
#include <gouraudshader.h>

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
    void initializePerspectiveMatrix();
    void initializeObject();
    void initializeCube();
    void initializePyramid();
    void initializeLight();


private:
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation
    Cube cube;
    Pyramid pyramid;
    ImportedObject object;

    // Shader programs
    Shader* currentShader;
    NormalShader normalShader;
    GouraudShader gouraudShader;
    PhongShader phongShader;
    void setDataToUniform();

    // Transformation matrices
    QMatrix4x4 perspectiveTransformationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 scalingMatrix;
    QMatrix4x4 cubeTranslationMatrix;
    QMatrix4x4 pyramidTranslationMatrix;
    QMatrix4x4 objectTranslationMatrix;

    // Painting methods
    void paintCube();
    void paintPyramid();
    void paintObject();

    void createShaderPrograms(ShadingMode shadingMode);
};

#endif // MAINVIEW_H
