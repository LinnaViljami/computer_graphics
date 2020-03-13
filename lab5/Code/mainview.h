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
#include "TextureType.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum SceneObject {
    // add new object types between first and last
    FirstSceneObject,
    Goat,
    MySphere,
    LastSceneObject};

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(Shader::ShadingMode shading);

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
    void initializeObjects();
    void initializeObject(SceneObject objectId, ImportedObjectType type, TextureType objectTexture);


private:
    std::map<SceneObject, ImportedObject> objects;
    QOpenGLDebugLogger debugLogger;
    QTimer timer; // timer used for animation
    float rotationAngle;

    // Shader programs
    Shader* currentShader;
    NormalShader normalShader;
    PhongShader phongShader;
    QVector<quint8> textureData;
    void setDataToUniform(SceneObject objectId);

    QVector3D getLightPosition();
    QVector3D getLightColor();
    // Transformation matrices
    QMatrix4x4 perspectiveTransformationMatrix;


    std::map<TextureType, GLuint> textureNames;
    GLuint getTextureName(TextureType textureType);
    void loadTextures();
    void loadTexture(TextureType textureType, GLuint& texturePtr);
    void initializeAnimationTimer();
    void rotateCamera();

    // Painting methods
    void paintObject(SceneObject objectId);

    void createShaderPrograms(Shader::ShadingMode shadingMode);

    QString getTextureFileName(TextureType texture);
};

#endif // MAINVIEW_H
