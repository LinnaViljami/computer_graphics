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
#include "watershader.h"
#include "TextureType.h"
#include "animation.h"

enum SceneObject {
    // add new object types between first and last
    FirstSceneObject,
    CatDIff,
    RugCat,
    MySphere,
    Goat,
    Water,
    LastSceneObject};

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:

    MainView(QWidget *parent = nullptr);
    ~MainView();

    // Functions for widget input events
    void setRotationToAllObjects(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);

    // Camera movement
    bool movingForwards, movingBackwards, movingLeft, movingRight;
    bool previousMouseCoordinatesKnown;
    int previousMouseX, previousMouseY;
    int mouseX, mouseY;

    QVector<quint8> imageToBytes(QImage image);
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


private:
    void initializeObjects();
    void initializeObject(SceneObject objectId, ImportedObjectType type, TextureType objectTexture);
    void initializeAnimations();
    void linkShaderToObject(SceneObject objectId);
    std::map<SceneObject, ImportedObject> objects;
    std::map<SceneObject, std::shared_ptr<Animation>> animations;
    std::map<SceneObject, Shader*> objectShaders;
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation
    float rotationAngle;
    float cameraX;
    float cameraZ;
    QVector3D cameraPosition;
    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;

    // Shader programs
    NormalShader normalShader;
    PhongShader phongShader;
    WaterShader waterShader;
    QVector<quint8> textureData;
    void setDataToUniform(SceneObject objectId);

    QVector3D getLightPosition();
    QVector3D getLightColor();
    // Transformation matrices
    QMatrix4x4 viewTransformationMatrix;
    QMatrix4x4 perspectiveTransformationMatrix;


    std::map<TextureType, GLuint> textureNames;
    GLuint getTextureName(TextureType textureType);
    void loadTextures();
    void loadTexture(TextureType textureType, GLuint& texturePtr);
    void initializeAnimationTimer();
    void initializeCameraPosition();

    // Camera position and movement
    float pitch, yaw, roll;
    void moveForwards();
    void moveBackwards();
    void moveLeft();
    void moveRight();
    void updatePosition();
    void updateCamera();
    void animateObjects();
    // Painting methods
    void paintObject(SceneObject objectId);

    void createShaderPrograms();

    QString getTextureFileName(TextureType texture);
};

#endif // MAINVIEW_H
