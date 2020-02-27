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
    void setUniformLocations();
    void initializePerspectiveMatrix();
    void initializeObject();
    void initializeCube();
    void initializePyramid();


private:
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation
    Cube cube;
    Pyramid pyramid;
    ImportedObject object;

    // Shader programs
    QOpenGLShaderProgram normalShaderProgram;
    QOpenGLShaderProgram phongShaderProgram;
    QOpenGLShaderProgram gouraudShaderProgram;

    // Transformation matrices
    QMatrix4x4 perspectiveTransformationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 scalingMatrix;
    QMatrix4x4 cubeTranslationMatrix;
    QMatrix4x4 pyramidTranslationMatrix;
    QMatrix4x4 objectTranslationMatrix;

    // --- Uniform locations ---
    // Phong shading
    GLint phongShadingTransformationUniformLocation;
    GLint phongShadingProjectionUniformLocation;
    GLint phongShadingNormalTransformationUniformLocation;
    GLint phongShadingMaterialUniformLocation;
    GLint phongShadingLightPositionUniformLocation;
    // Normal shading
    GLint normalShadingTransformationUniformLocation;
    GLint normalShadingProjectionUniformLocation;
    GLint normalShadingNormalTransformationUniformLocation;
    GLint normalShadingMaterialUniformLocation;
    GLint normalShadingLightPositionUniformLocation;
    // Gouraud shading
    GLint gouraudShadingTransformationUniformLocation;
    GLint gouraudShadingProjectionUniformLocation;
    GLint gouraudShadingNormalTransformationUniformLocation;
    GLint gouraudShadingMaterialUniformLocation;
    GLint gouraudShadingLightPositionUniformLocation;

    GLint getCurrentTransformationUniformLocation();
    GLint getCurrentProjectionUniformLocation();
    GLint getCurrentNormalTransformationUniformLocation();
    GLint getCurrentMaterialUniformLocation();
    ShadingMode getCurrentShaderType();

    QOpenGLShaderProgram* currentShaderProgram;

    // Painting methods
    void paintCube();
    void paintPyramid();
    void paintObject();

    void createShaderPrograms(ShadingMode shadingMode);
};

#endif // MAINVIEW_H
