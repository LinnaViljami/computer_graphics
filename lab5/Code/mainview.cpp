#include "mainview.h"
#include <QDateTime>
#include <QtMath>
#include <cstdlib>


/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent){
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    qDebug() << "MainView destructor";
    for(auto& object : objects){
        glDeleteBuffers(1, &object.second.vboId);
        glDeleteVertexArrays(1, &object.second.vaoId);
    }
    makeCurrent();
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage)),
            this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }


    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background color.
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
    createShaderPrograms(Shader::PHONG);
    initializeObjects();
    loadTextures();
    initializeCameraPosition();
    initializeAnimationTimer();
}

// Adds and links a vertex shader and a fragment shader, based on which ShaderType
// is passed as parameter.
void MainView::createShaderPrograms(Shader::ShadingMode shadingMode) {
    phongShader.init();
    normalShader.init();
    setShadingMode(shadingMode);
}

QString MainView::getTextureFileName(TextureType texture)
{
    QString fileName = "no filename";
    int width = 512;
    int height = 1024;
    switch (texture) {
    case NoTexture:
        break;
    case LastTexture:
        break;
    case Diff:
        fileName = ":/textures/cat_diff.png";
        break;
    case Norm:
        fileName = ":/textures/cat_norm.png";
        break;
    case Spec:
        fileName = ":/textures/cat_spec.png";
        break;
    case Rug:
        fileName = ":/textures/rug_logo.png";
        width = 1024;
        break;
    }
    return fileName;
}


void MainView::initializeObject(SceneObject objectId, ImportedObjectType type, TextureType objectTexture) {


    objects[objectId] = ImportedObject(type, objectTexture);
    ImportedObject* object = &objects.at(objectId);
    glGenBuffers(1, &object->vboId);
    glGenVertexArrays(1, &object->vaoId);

    glBindVertexArray(object->vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, object->vboId);

    glBufferData(GL_ARRAY_BUFFER, object->vertices.size()*sizeof(vertex3d), object->vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLintptr coordinatPtrIndex = 0*sizeof(float);
    GLintptr colorPtrIndex = offsetof(vertex3d, normalX);
    GLintptr texturePtrIndex = offsetof(vertex3d, textureX);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex3d), (GLvoid*)(coordinatPtrIndex));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex3d), (GLvoid*)(colorPtrIndex));
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, sizeof(vertex3d), (GLvoid*)(texturePtrIndex));

}


void MainView::setDataToUniform(SceneObject objectId)
{
    ImportedObject& object = objects.at(objectId);
    bool useTextures = (object.textureType != TextureType::NoTexture);
    switch (currentShader->type()) {
    case Shader::PHONG:
        phongShader.setUniformData(object.getModelTransformationMatrix(),
                                   viewTransformationMatrix,
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix(),
                                   object.getMaterialVector(),
                                   getLightPosition(),
                                   getLightColor(),
                                   useTextures);
        break;
    case Shader::NORMAL:
        normalShader.setUniformData(object.getModelTransformationMatrix(),
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix());
        break;
    case Shader::GOURAUD:
        qDebug("gouraud shader not implemented");
        break;
    }
}

QVector3D MainView::getLightPosition()
{
    return {
        0.0f,
        100.0f,
        0.0f
    };
}

QVector3D MainView::getLightColor()
{
    return {
        1.0f,
        1.0f,
        1.0f
    };
}

GLuint MainView::getTextureName(TextureType textureType)
{
    return textureNames.at(textureType);
}

void MainView::loadTextures() {
    for (int textureTypeInt = TextureType::NoTexture; textureTypeInt != TextureType::LastTexture; textureTypeInt++){
        TextureType textureType = TextureType(textureTypeInt);
        textureNames[textureType] = GLuint();
        GLuint& textureName = textureNames.at(textureType);
        loadTexture(textureType, textureName);
    }
}

void MainView::loadTexture(TextureType textureType, GLuint& textureName)
{

        if(textureType != TextureType::NoTexture){
            QString filename = getTextureFileName(textureType);

            glGenTextures(1, &textureName);

            glBindTexture(GL_TEXTURE_2D, textureName);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            // Push image data to texture.
            QImage image(filename);
            QVector<quint8> imageData = imageToBytes(image);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
                         0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
}



}

void MainView::initializeAnimationTimer() {
    rotationAngle = 0.0f;
    timer.start(1000.0 / 60.0);
}

void MainView::initializeCameraPosition() {
    cameraX = 0.0f;
    cameraZ = -10.0f;
}

void MainView::updateCameraPosition() {
    //    // Performs the Gram–Schmidt process
    QVector3D cameraTarget(0.0f, 0.0f, 0.0f);
    QVector3D cameraPosition(cameraX, 0.0f, cameraZ);
    QVector3D up(0.0f, 1.0f, 0.0f);

    // This value is in the direction of the negative z-axis.
    QVector3D cameraDirection = (cameraTarget - cameraPosition).normalized();
    QVector3D cameraRight = QVector3D::crossProduct(up, cameraDirection);
    QVector3D cameraUp = QVector3D::crossProduct(cameraDirection, cameraRight);

    // Make camera spin
//    const float radius = 10.0f;
//    float cameraX = sin(rotationAngle) * radius;
//    float cameraZ = cos(rotationAngle) * radius;

    // Calculate camera view
//    QVector3D eye(cameraX, 0.0f, cameraZ);
//    QVector3D center(0.0f, 0.0f, 0.0f);

    QMatrix4x4 viewAngle;
    viewAngle.setToIdentity();
    viewAngle.setRow(0, QVector4D(cameraRight, 0.0f));
    viewAngle.setRow(1, QVector4D(cameraUp, 0.0f));
    viewAngle.setRow(2, QVector4D(cameraDirection, 0.0f));
    viewAngle.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    QMatrix4x4 viewPosition;
    viewPosition.setToIdentity();
    viewPosition.setRow(0, QVector4D(1.0f, 0.0f, 0.0f, cameraPosition.x()));
    viewPosition.setRow(1, QVector4D(0.0f, 1.0f, 0.0f, cameraPosition.y()));
    viewPosition.setRow(2, QVector4D(0.0f, 0.0f, 1.0f, cameraPosition.z()));
    viewPosition.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    viewTransformationMatrix = viewAngle * viewPosition;
}


void MainView::moveForwards() {
    cameraZ += 0.5f;
}

void MainView::moveBackwards() {
    cameraZ -= 0.5f;
}


void MainView::paintObject(SceneObject objectId)
{
    ImportedObject& object = objects.at(objectId);
//    qDebug("Paint object called");
    object.setTranslation(0, -3, -10);

    GLint * textureUniformLocation = currentShader->getTextureBufferLocation();
    if(textureUniformLocation != nullptr && object.textureType != TextureType::NoTexture){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureNames.at(object.textureType));
        glUniform1i(*textureUniformLocation, 0);

    }
    setDataToUniform(objectId);
    glBindVertexArray(object.vaoId);
    glDrawArrays(GL_TRIANGLES, 0, object.vertices.size());
}


// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    currentShader->bind();
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Drive animations
    rotationAngle += 1.0f;
    if (rotationAngle >= 3600) rotationAngle -= 3600;
    updateCameraPosition();

    setRotation(0, rotationAngle, 0);

    paintObject(SceneObject::Goat);
    paintObject(SceneObject::MySphere);
    currentShader->release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    qDebug() << "resize called.";
    perspectiveTransformationMatrix.setToIdentity();
    perspectiveTransformationMatrix.perspective(60, ((float)newWidth)/newHeight, 0.2f, 20.0f);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    for (auto& object : objects){
        object.second.setRotation(rotateX, rotateY, rotateZ);
    }

    update();
}

void MainView::setScale(int scale) {
    for (auto& object : objects){
        object.second.setScale(scale);
    }
    update();
}

void MainView::setShadingMode(Shader::ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    switch (shading) {
    case Shader::PHONG:
        currentShader = &phongShader;
        break;
    case Shader::NORMAL:
        currentShader = &normalShader;
        break;
    case Shader::GOURAUD:
        qDebug("gouraud shader not implemented");
        break;
    }
}

// --- Private helpers ---

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}

void MainView::initializeObjects()
{
    for (int sceneObjectInt = SceneObject::FirstSceneObject; sceneObjectInt != SceneObject::LastSceneObject; sceneObjectInt++){
        SceneObject objectId = (SceneObject)sceneObjectInt;
        switch (objectId) {
        case FirstSceneObject:
            break;
        case MySphere:
            initializeObject(objectId, ImportedObjectType::sphere, TextureType::NoTexture);
            break;

        case Goat:
            initializeObject(objectId, ImportedObjectType::cat, TextureType::Diff);
            break;

        case LastSceneObject:
            break;
        }
    }
}
